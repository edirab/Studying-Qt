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
}

void MyScene::keyPressEvent(QKeyEvent *event) {
    auv->keyboardRedraw(event);
}
