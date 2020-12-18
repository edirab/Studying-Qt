#include "scene.h"

MyScene::MyScene(QWidget *parent)
    : QGraphicsScene(parent)
{
    mWidth = parent->width();
    mHeight = parent->height();

    this->setSceneRect(0, 0, mWidth, mHeight);

    this->mGroupStation = new QGraphicsItemGroup();
    this->mGroupAUV = new QGraphicsItemGroup();

    Station = new chargingStation(this, mGroupStation, this);
    //Station->setParentItem(scene);
    this->addItem(Station);
    Station->setPos(mWidth/2, mHeight/2);

    trajectory = new Trajectory(this, this);
    trajectory->setParentItem(Station);
    this->addItem(trajectory);
    trajectory->setPos(0, 0);

    auv = new AUV(this, mGroupAUV, this);
    auv->setParentItem(Station);
    this->addItem(auv);
    auv->setPos(0, 0);

    vAngle = new viewingAngle(this, 50);
    vAngle->setParentItem(auv);
    this->addItem(vAngle);
    vAngle->setPos(auv->mLen/2, 0);

    animTimer = new QTimer;
    animTimer->setInterval(this->animationStep);

    bool bOk = QObject::connect(this->animTimer, &QTimer::timeout, this, &MyScene::AnimationStep);
    Q_ASSERT(bOk);
}

void MyScene::keyPressEvent(QKeyEvent *event) {
    auv->keyboardRedraw(event);
}


/*
    Slot 1
*/
void MyScene::readFile(){
    qDebug() << "In Read file \n";

    data.clear();
    bool dataIsCoorect = true;

    QString file_path = QFileDialog::getOpenFileName(0, "Выберите файл с данными", "../docking/", "*.dat *.txt");
    QFile file(file_path);

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "error " << file.errorString() << "\n";
    }

    int line_number = 1;
    QTextStream in(&file);

    while(!in.atEnd()) {
        QVector<float> one_line(4);

        QString line = in.readLine();
        QStringList fields = line.split(" ");
        //qDebug() << fields << "\n";

        if (fields.length() == 4) {
            one_line[0] = fields.at(0).toDouble();
            one_line[1] = fields.at(1).toDouble();
            one_line[2] = fields.at(2).toDouble();
            one_line[3] = fields.at(3).toDouble();
            line_number++;
        } else {
            dataIsCoorect = false;
                QString error_msg = "";
                error_msg += "<p> В файле <b>" + file_path + "</b> обнаружена ошибка на строке </p>";
                error_msg += "<p>" + QString::number(line_number) + ": " + QString(line) + "</p>";
                error_msg +="<p> Ожидается 4 числовых параметра вместо " + QString::number(fields.length()) + ".</p>";
                error_msg += "<p>Проверьте целостность файла и повторите попытку.</p>";
                qDebug() << error_msg;
            emit fileReadFailed(error_msg);
            break;
        }
        //qDebug() << a << "\n";
        this->data.append(one_line);
        this->trajectory->data.append(one_line);
    }

    file.close();
    for (int i = 0; i < data.length(); i++){
        //qDebug() << "Data: " << this->data[i];
    }

    if (dataIsCoorect){
        emit fileReadSuccessful();
    }
}

void msleep(int ms)
{
    QThread::msleep(ms);
}


void MyScene::AnimationStep(){


    qDebug() << "Animation step:" << animIteration << "\n";

    if (animIteration < 8000){
        // Time, X (up), Z(right), Yaw (Counter Clockwise)
        float X_ = data[animIteration][1] ;
        float Z_ = data[animIteration][2] ;
        float Yaw = data[animIteration][3];

        int x_projection = int(Z_ * 0.5 * SCALE_FACTOR);
        int y_projection = - int(X_ * 0.5 * SCALE_FACTOR) ;
        int yaw_screen = ( Yaw - 90); // Потому что изначально аппарат был нарисован боком.
        qDebug() << x_projection << " " << y_projection << " " << yaw_screen << " " << Yaw;

        auv->setPos(x_projection, y_projection);
        auv->setRotation( yaw_screen );

        emit sendCoordsDuringAnimation(X_, Z_, Yaw);
        emit sendCurrentIterationStep(animIteration, 8000);
        //this->update(0, 0, this->width(), this->height());
        animIteration++;
    }
    else {
        animTimer->stop();
    }
}


/*
    Slot 2
*/
void MyScene::startVisualization(){

    animTimer->start();
    qDebug() << "Inside Visualization \n";
}



/*
    Slot 2
*/
void MyScene::drawTrajectory(){

    qDebug() << "Здесь отобразим траекторию";
}















