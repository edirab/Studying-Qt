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

    trajectory = new Trajectory(this, this, &animIteration);
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
    bOk = bOk && QObject::connect(&(this->auv->su), &SU_ROV::sendComputedCoords, this->trajectory, &Trajectory::receiveComputedCoords);
    Q_ASSERT(bOk);
}

void MyScene::keyPressEvent(QKeyEvent *event) {
    auv->keyboardRedraw(event);
}



void msleep(int ms)
{
    QThread::msleep(ms);
}


/*
    Slot 2
*/
void MyScene::AnimationStep(){


    qDebug() << "Animation step:" << animIteration << "\n";

    if (this->trajectory->data.size() > 0){
        // Time, X (up), Z(right), Yaw (Counter Clockwise)
        float X_ = this->trajectory->data.back().X;
        float Z_ = this->trajectory->data.back().Z;
        float Yaw = this->trajectory->data.back().Yaw;

        int x_projection = int(Z_ * 0.5 * SCALE_FACTOR);
        int y_projection = - int(X_ * 0.5 * SCALE_FACTOR) ;
        int yaw_screen = ( Yaw - 90); // Потому что изначально аппарат был нарисован боком.
        //qDebug() << x_projection << " " << y_projection << " " << yaw_screen << " " << Yaw;

        auv->setPos(x_projection, y_projection);
        auv->setRotation( yaw_screen );

        emit sendCoordsDuringAnimation(X_, Z_, Yaw);
        emit sendCurrentIterationStep(animIteration, 8000);
        //this->update(0, 0, this->width(), this->height());
        animIteration++;
    }
}


/*
    Slot 3
*/
void MyScene::startAminTimer(){

    this->animTimer->start();
    this->auv->su.timer.start();
    qDebug() << "Запуск таймера \n";
}



/*
    Slot 5
*/
void MyScene::stopAnimTimer(){

    animTimer->stop();
    this->auv->su.timer.stop();
    qDebug() << "Останов таймера \n";
}







