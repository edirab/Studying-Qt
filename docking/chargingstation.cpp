#include "chargingstation.h"

chargingStation::chargingStation(QObject *parent) : QObject(parent)
{

}


chargingStation::chargingStation(QGraphicsScene *s, QGraphicsItemGroup *g, QObject *parent): QObject(parent)
{
    this->scene = s;
    this->group = g;
   // this->parent();

    int width = scene->width();
    int height = scene->height();
    qDebug() << "Размеры GraphicsView: "<< width << " " << height;

    this->screenX = width / 2;
    this->screenY = height / 2;
}

void chargingStation::draw(){

    QPen penBlack(Qt::black); // Задаём чёрную кисть
    QPen penRed(Qt::red);   // Задаём красную кисть

    group->addToGroup(scene->addRect(screenX-dockSize/2,
                                  screenY-dockSize/2,
                                  dockSize, dockSize, penBlack));

    group->addToGroup(scene->addRect(screenX-dockSize/4,
                                  screenY-dockSize/4,
                                  dockSize/2, dockSize/2, penBlack));

    // 1
    group->addToGroup(scene->addLine(screenX-dockSize/2,
                                  screenY-dockSize/2,
                                  screenX-dockSize/4,
                                  screenY-dockSize/4, penBlack));
    // 2
    group->addToGroup(scene->addLine(screenX-dockSize/2 + dockSize,
                                  screenY-dockSize/2,
                                  screenX-dockSize/4 + dockSize / 2,
                                  screenY-dockSize/4, penBlack));

    // 3
    group->addToGroup(scene->addLine(screenX-dockSize/2,
                                  screenY+dockSize/2,
                                  screenX-dockSize/4,
                                  screenY+dockSize/4, penBlack));

    // 4
    group->addToGroup(scene->addLine(screenX+dockSize/2,
                                  screenY+dockSize/2,
                                  screenX+dockSize/2 - dockSize/4,
                                  screenY+dockSize/4, penBlack));

}

