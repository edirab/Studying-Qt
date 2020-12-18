#include "trajectory.h"

Trajectory::Trajectory(QGraphicsScene *s, QObject *parent):
    QObject(parent), QGraphicsItem()
{
    this->scene = s;

}


QRectF Trajectory::boundingRect() const {

    int width = scene->width();
    int height = scene->height();

    return QRectF(0, 0, width, height);
}


void Trajectory::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*){

    painter->setPen(Qt::blue);
    painter->setBrush(Qt::transparent);

    //painter->drawEllipse(-5, -5, 5, 5);
    //painter->drawRect(10, 10, 200, 200);
    int x_projection_prev = 0;
    int y_projection_prev = 0;

    if (data.size()){

        float X_ = data[0][1] ;
        float Z_ = data[0][2] ;

        x_projection_prev = int(Z_ * 0.5 * SCALE_FACTOR);
        y_projection_prev = - int(X_ * 0.5 * SCALE_FACTOR) ;
    }

    for (int i = 0; i < data.size() - 1; i++) {

        float X_ = data[i][1] ;
        float Z_ = data[i][2] ;

        int x_projection = int(Z_ * 0.5 * SCALE_FACTOR);
        int y_projection = - int(X_ * 0.5 * SCALE_FACTOR) ;

        painter->drawLine(x_projection_prev, y_projection_prev,
                          x_projection, y_projection);

        x_projection_prev = x_projection;
        y_projection_prev = y_projection;
    }
}
