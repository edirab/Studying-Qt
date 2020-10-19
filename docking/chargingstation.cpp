#include "chargingstation.h"

chargingStation::chargingStation(QObject *parent) : QObject(parent), QGraphicsItem()
{

}


chargingStation::chargingStation(QGraphicsScene *s, QGraphicsItemGroup *g, QObject *parent):
    QObject(parent), QGraphicsItem()
{
    this->scene = s;
    this->group = g;
   // this->parent();

    int width = scene->width();
    int height = scene->height();

    qDebug() << "Размеры GraphicsView: "<< width << " " << height;
    //qDebug() << this->parentItem()->x();

    this->screenX = width / 2;
    this->screenY = height / 2;
}


QRectF chargingStation::boundingRect() const {
    int L = dockSize;
    return QRectF(-L/2, -L/2, L, L);
}

void chargingStation::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*){

    painter->setPen(Qt::black);
    painter->setBrush(Qt::white);

    int L = dockSize;

    painter->drawRect(-L/2, -L/2, L, L);
    painter->drawLine(-L/2, -L/2, L/2, L/2);
    painter->drawLine(-L/2, L/2, L/2, -L/2);

    painter->drawRect(-L/4, -L/4, L/2, L/2);
}

