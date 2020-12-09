#include "scene.h"

MyScene::MyScene(QWidget *parent)
    : QGraphicsScene(parent)
{
    mW = parent->width();
    mH = parent->height();

    this->setSceneRect(0, 0, mW, mH);

    this->group_1 = new QGraphicsItemGroup();
    this->group_2 = new QGraphicsItemGroup();

    //this->addEllipse(0, 0, 100, 100, QPen(Qt::red));

    Station = new chargingStation(this, group_1, this);
    //Station->setParentItem(scene);
    this->addItem(Station);
    Station->setPos(mW/2, mH/2);

    auv = new AUV(this, group_2, this);
    auv->setParentItem(Station);
    this->addItem(auv);
    auv->setPos(0, 0);

    vAngle = new viewingAngle(this, 50);
    vAngle->setParentItem(auv);
    this->addItem(vAngle);
    vAngle->setPos(auv->mLen/2, 0);

    //bool bOk = QObject::connect(ui->pushButton_loadFile, &QPushButton::clicked, this, &MainWindow::readFile);
    //Q_ASSERT(bOk);
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
}

void msleep(int ms)
{
    QThread::msleep(ms);
}


/*
    Slot 2
*/
void MyScene::startVisualization(){
    qDebug() << "Inside Visualization \n";

//    for(int i = 0; i < this->data.length(); i++){

//        //float x_projection = stepLin * cosf(qDegreesToRadians(mWorldYaw+90));
//        //float y_projection = stepLin * sinf(qDegreesToRadians(mWorldYaw+90));

//        // Time, X (up), Z(right), Yaw (Counter Clockwise)
//        int x_projection = int(data[i][1] * 50);
//        int y_projection = int(data[i][2] * 50);
//        qDebug() << x_projection << " " << y_projection;
//        auv->setPos(x_projection, y_projection);
//        this->update(0, 0, this->width(), this->height());
//        msleep(5);
//    }
    //auv->moveBy(50, 50);
//    auv->setPos(50, 50);
//    this->update(0, 0, this->width(), this->height());
//    msleep(50);
//    auv->setPos(0, 0);
//    this->update(0, 0, this->width(), this->height());
//    msleep(50);
//    auv->setPos(50, 50);
//    this->update(0, 0, this->width(), this->height());
//    msleep(50);
//    auv->setPos(0, 0);
//    this->update(0, 0, this->width(), this->height());
//    msleep(50);
//    auv->setPos(50, 50);
//    this->update(0, 0, this->width(), this->height());
//    msleep(50);
//    auv->setPos(0, 0);
//    this->update(0, 0, this->width(), this->height());
//    msleep(50);

    static int i = 0;
     //Time, X (up), Z(right), Yaw (Counter Clockwise)
    int x_projection = int(data[i][1] * 10);
    int y_projection = int(data[i][2] * 10);
    qDebug() << x_projection << " " << y_projection;
    auv->setPos(x_projection, y_projection);
    this->update(0, 0, this->width(), this->height());
    msleep(5);
    i++;

}




















