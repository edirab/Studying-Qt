#include "auv.h"

AUV::AUV(QObject *parent) : QObject(parent)
{

}


AUV::AUV(QGraphicsScene *s, QGraphicsItemGroup *g, QObject *parent):
    QObject(parent), QGraphicsItem()
{
    this->scene = s;
    this->group = g;

    int width = scene->width();
    int height = scene->height();

    this->auv_x = width - 3 * width / 5;
    this->auv_y = height -  height / 4;

    if (this->parentItem() != nullptr){
        this->parentItem()->x();
        this->parentItem()->y();
        qDebug() <<  this->parentItem()->x() << " " << this->parentItem()->y() ;
    }
}

QRectF AUV::boundingRect() const {
  return QRectF(QPoint(-auvLen/2, -auvDiam/2),
                QPoint(auvLen+auvDiam/2, auvDiam));
}


void AUV::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*){

    painter->setPen(Qt::black);
    painter->setBrush(Qt::yellow);

    //painter->drawPoint(0, 0);
    //painter->drawEllipse(-5, -5, 5, 5);

    painter->drawEllipse(auvLen/2 - auvDiam/2,
                         -auvDiam/2,
                         auvDiam,
                         auvDiam);

    painter->drawRect(-auvLen/2,
                      -auvDiam/2,
                      auvLen,
                      auvDiam);
}

void AUV::truncateYaw(){
    if ( auvYaw >= 360){
        this->auvYaw -= 360;
    }else if (auvYaw < 0 ){
        this->auvYaw += 360;
    }
}


void AUV::keyboardRedraw(QKeyEvent *event){

    int key=event->nativeVirtualKey(); //event->key() - целочисленный код клавиши
    bool useArrows = false;

    if (useArrows){
        switch (key) {
            case Qt::Key_Right:
                auv_x -= 10;
            break;

            case Qt::Key_Left:
                auv_x -= 10;
            break;

            case Qt::Key_Up:
                auv_y -= 10;
            break;

            case Qt::Key_Down:
                auv_y += 10;
            break;

            default:
                qDebug() << "Unknown";
            break;
        }
    }

        if (key == Qt::Key_E) {
            //qDebug() << "Native key: " << event->nativeVirtualKey();
            float x_projection = stepLin * cosf(qDegreesToRadians(auvYaw+90));
            float y_projection = stepLin * sinf(qDegreesToRadians(auvYaw+90));
            this->moveBy(x_projection, y_projection);
        }
        else if (key == Qt::Key_Q) {
            float x_projection = stepLin * cosf(qDegreesToRadians(auvYaw-90));
            float y_projection = stepLin * sinf(qDegreesToRadians(auvYaw-90));
            this->moveBy(x_projection, y_projection);

        }
        else if (key == Qt::Key_W) {
            float x_projection = stepLin * cosf(qDegreesToRadians(auvYaw));
            float y_projection = stepLin * sinf(qDegreesToRadians(auvYaw));
            //qDebug() << "x+proj: " << x_projection << "y_proj" << y_projection;
            this->moveBy(x_projection, y_projection);
        }
        else if (key == Qt::Key_S) {

           float x_projection = stepLin * cosf(qDegreesToRadians(auvYaw));
           float y_projection = stepLin * sinf(qDegreesToRadians(auvYaw));
           //qDebug() << "x+proj: " << x_projection << "y_proj" << y_projection;
           this->moveBy(- x_projection, - y_projection);
        }
        else if (key == Qt::Key_A){
            auvYaw -= stepRot;
            truncateYaw();
            this->setRotation(auvYaw);
        }
        else if (key == Qt::Key_D){
            auvYaw += stepRot;
            truncateYaw();
            this->setRotation(auvYaw);
        }

        auv_x = this->x();
        auv_y = this->y();

        emit sendCoords(auv_x, auv_y, auvYaw);
        if (auvDebug){
            qDebug() << "Нажатие: " << key << "x: " << auv_x << "y" << auv_y << "yaw: " <<auvYaw ;
        }

      //this->scene->update(0, 0, scene->width(), scene->height());
}
