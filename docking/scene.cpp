#include "scene.h"

MyScene::MyScene(QWidget *parent)
    : QGraphicsScene(parent)
{
    mWidth = parent->width();
    mHeight = parent->height();

    this->setSceneRect(0, 0, mWidth, mHeight);

    this->mGroupStation = new QGraphicsItemGroup();
    this->mGroupAUV = new QGraphicsItemGroup();

    //this->addEllipse(0, 0, 100, 100, QPen(Qt::red));

    Station = new chargingStation(this, mGroupStation, this);
    //Station->setParentItem(scene);
    this->addItem(Station);
    Station->setPos(mWidth/2, mHeight/2);

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

    // открываем файл
    QFile file("../coords.dat");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "error " << file.errorString() << "\n";
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QVector<float> a(4);

        QString line = in.readLine();
        QStringList fields = line.split(" ");
        //qDebug() << fields << "\n";

        a[0] = fields.at(0).toDouble();
        a[1] = fields.at(1).toDouble();
        a[2] = fields.at(2).toDouble();
        a[3] = fields.at(3).toDouble();
        //qDebug() << a << "\n";
        this->data.append(a);
    }

    file.close();

    for (int i = 0; i < data.length(); i++){
        qDebug() << "Data: " << this->data[i];
    }
    emit fileReadSuccessful();
}

void msleep(int ms)
{
    QThread::msleep(ms);
}


void MyScene::AnimationStep(){

    static int i = 0;
    qDebug() << "Animation step:" << i << "\n";

    if (i < 8000){
        // Time, X (up), Z(right), Yaw (Counter Clockwise)
        float X_ = data[i][1] * 66.7;
        float Z_ = data[i][2] * 66.7;

        int x_projection = int(Z_);
        //int y_projection = (this->mH - int(float(X_)) ) ;
        int y_projection = (- int(float(X_)) ) ;

        qDebug() << x_projection << " " << y_projection;
        auv->setPos(x_projection, y_projection);
        this->update(0, 0, this->width(), this->height());
        i++;
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




















