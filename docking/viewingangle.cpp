#include "viewingangle.h"

viewingAngle::viewingAngle(QObject *parent) : QObject(parent)
{

}


viewingAngle:: viewingAngle(QObject *parent, int angle):
                            QObject(parent), QGraphicsItem(), mAngle(angle)
{

}


void viewingAngle::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*){
    //qDebug() <<  120 * tanf(qDegreesToRadians(angle/2));
    painter->setPen(Qt::green);
    painter->drawLine(0, 0, 120, 120 * tanf(qDegreesToRadians(mAngle/2)) );
    painter->drawLine(0, 0, 120, -120 * tanf(qDegreesToRadians(mAngle/2)));
}

QRectF viewingAngle::boundingRect() const {
    return QRectF(QPointF(0, -120 *  tanf(qDegreesToRadians(mAngle/2))),
                  QPointF(120, 2 * 120 * tanf(qDegreesToRadians(mAngle/2))));
}
