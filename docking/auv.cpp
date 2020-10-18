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

    int width = scene->width();
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

    if (auvDebug){
        int width = scene->width();//  100;
        int height = scene->height();
        qDebug() << "Размеры GraphicsView: "<< width << " " << height;
    }

    group->addToGroup(scene->addRect(auv_x - auvLen/2,
                                  auv_y - auvDiam/2,
                                  auvLen, auvDiam, penBlack, brushYellow));
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
        }
        else if (key == Qt::Key_A) {
           auv_x -= 10;
        }
        else if (key == Qt::Key_W) {
           auv_y -= 10;
        }
        else if (key == Qt::Key_S) {
           auv_y += 10;
        }

      qDebug() << "auv_x: " << auv_x << "auv_y" << auv_y ;

      foreach( QGraphicsItem *item, scene->items(group->boundingRect())) {
         if(item->group() == group ) {
            delete item;
         }
      }

      this->draw();
}
