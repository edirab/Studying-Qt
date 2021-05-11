#include "trajectory.h"

Trajectory::Trajectory(QGraphicsScene *s, QObject *parent, int* animIteration):
    QObject(parent), QGraphicsItem(), animIteration_ptr(animIteration)
{
    this->scene = s;

}

void Trajectory::receiveComputedCoords(float X, float Z, float Yaw){

//    qDebug() << "In receiveComputedCoords: " << this->data.size()
//             << " " << X
//             << " " << Z
//             << " " << Yaw << " " << "\n";

    if (X != 15 && Z != 0){

        Coords c;
        c.X = X;
        c.Z = Z;
        c.Yaw = Yaw;

        this->data.append(c);
    }
}

QRectF Trajectory::boundingRect() const {

    int width = scene->width();
    int height = scene->height();

    return QRectF(-width/2, -height/2, width/2, height/2);
}

/*
Сделано неоптимально, программа тормизит
*/
void Trajectory::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*){

//    int width = scene->width();
//    int height = scene->height();
//    this->update(-width/2, -height/2, width/2, height/2);

    //qDebug() << "Anim step in Trajectory" << *animIteration_ptr << "\n";
    painter->setPen(Qt::blue);
    painter->setBrush(Qt::transparent);

    //painter->drawEllipse(-5, -5, 5, 5);
    //painter->drawRect(10, 10, 200, 200);
    int x_projection_prev = 0;
    int y_projection_prev = 0;

    if (data.size()){

        float X_ = data[0].X;
        float Z_ = data[0].Z;

        x_projection_prev = int(Z_ * 0.5 * SCALE_FACTOR);
        y_projection_prev = - int(X_ * 0.5 * SCALE_FACTOR) ;


        for (int i = 0; i < data.size(); i++) {

            float X_ = data[i].X;
            float Z_ = data[i].Z;

            int x_projection = int(Z_ * 0.5 * SCALE_FACTOR);
            int y_projection = - int(X_ * 0.5 * SCALE_FACTOR) ;

            painter->drawLine(x_projection_prev, y_projection_prev,
                              x_projection, y_projection);

            x_projection_prev = x_projection;
            y_projection_prev = y_projection;
        }
    }
}
