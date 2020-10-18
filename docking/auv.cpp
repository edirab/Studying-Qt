#include "auv.h"

AUV::AUV(QObject *parent) : QObject(parent)
{

}

//AUV::AUV(QGraphicsScene *s, QGraphicsItemGroup *g, QObject *parent){
//    this->scene = s;
//    this->group = g;
//    this->parent();
//}

AUV::AUV(QGraphicsScene *s, QGraphicsItemGroup *g, QObject *parent): QObject(parent)
{
    this->scene = s;
    this->group = g;
   // this->parent();

    int width = scene->width();//  100;
    int height = scene->height();

    this->auv_x= width - 3 * width / 5;
    this->auv_y = height -  height / 4;


}

void AUV::draw(){

    /* Приступаем к отрисовке произвольной картинки
     * */
    QPen penYellow(Qt::yellow);
    QPen penRed(Qt::red);
    QPen penBlack(Qt::black);

    QBrush brushYellow(Qt::yellow);

    int width = scene->width();//  100;
    int height = scene->height();
    qDebug() << "Размеры GraphicsView: "<< width << " " << height;

//    float centerx = width / 2;
//    float centery = height / 2;

//    int auvDiam = 30;
//    int auvLen = auvDiam * 4;

//    group->addToGroup(sc->addRect(width - 3 * width / 5,
//                                  height -  height / 4,
//                                  auvDiam * 4, auvDiam, penBlack, brushYellow));

    group->addToGroup(scene->addRect(auv_x - auvLen/2,
                                  auv_y - auvDiam/2,
                                  auvLen, auvDiam, penBlack, brushYellow));
}
