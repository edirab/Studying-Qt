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


void AUV::keyboardRedraw(QKeyEvent *event){

    int key=event->key(); //event->key() - целочисленный код клавиши
    qDebug() << "Нажатие: " << key;

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

        if (key == Qt::Key_D) {
           auv_x += 10;
           this->moveBy(10, 0);
        }
        else if (key == Qt::Key_A) {
           auv_x -= 10;
           this->moveBy(-10, 0);

        }
        else if (key == Qt::Key_W) {
           auv_y -= 10;
           this->moveBy(0, -10);
        }
        else if (key == Qt::Key_S) {
           auv_y += 10;
           this->moveBy(0, 10);
        }
        else if (key == Qt::Key_Q){
            auvYaw -= 5;
            this->setRotation(auvYaw);
        }
        else if (key == Qt::Key_E){
            auvYaw += 5;
            this->setRotation(auvYaw);
        }

      qDebug() << "auv_x: " << auv_x << "auv_y" << auv_y ;

      //this->scene->update(0, 0, scene->width(), scene->height());

}
