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

    this->mSceneX = width - 3 * width / 5;
    this->mSceneY = height -  height / 4;

    if (this->parentItem() != nullptr){
        this->parentItem()->x();
        this->parentItem()->y();
        qDebug() <<  this->parentItem()->x() << " " << this->parentItem()->y() ;
    }
}

QRectF AUV::boundingRect() const {
  return QRectF(QPoint(-mLen/2, -mDiam/2),
                QPoint(mLen+mDiam/2, mDiam));
}


void AUV::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*){

    painter->setPen(Qt::black);
    painter->setBrush(Qt::white);

    //painter->drawPoint(0, 0);
    //painter->drawEllipse(-5, -5, 5, 5);

    painter->drawEllipse(mLen/2 - mDiam/2, -mDiam/2, mDiam, mDiam);

    painter->setBrush(Qt::yellow);
    painter->drawRect(-mLen/2, -mDiam/2, mLen, mDiam);

    //this->scene->update(0, 0, scene->width(), scene->height());
    this->scene->update(100, 100, scene->width() - 100, scene->height() - 100);
}


void AUV::truncateYaw(){
    if ( mWorldYaw >= 360){
        this->mWorldYaw -= 360;
    }else if (mWorldYaw < 0 ){
        this->mWorldYaw += 360;
    }
}


void AUV::keyboardRedraw(QKeyEvent *event){

    int key=event->nativeVirtualKey(); //event->key() - целочисленный код клавиши
    bool useArrows = false;

    if (useArrows){
        switch (key) {
            case Qt::Key_Right:
                mSceneX -= 10;
            break;

            case Qt::Key_Left:
                mSceneX -= 10;
            break;

            case Qt::Key_Up:
                mSceneY -= 10;
            break;

            case Qt::Key_Down:
                mSceneY += 10;
            break;

            default:
                qDebug() << "Unknown";
            break;
        }
    }

        if (key == Qt::Key_E) {
            //qDebug() << "Native key: " << event->nativeVirtualKey();
            float x_projection = stepLin * cosf(qDegreesToRadians(mWorldYaw+90));
            float y_projection = stepLin * sinf(qDegreesToRadians(mWorldYaw+90));
            this->moveBy(x_projection, y_projection);
        }
        else if (key == Qt::Key_Q) {
            float x_projection = stepLin * cosf(qDegreesToRadians(mWorldYaw-90));
            float y_projection = stepLin * sinf(qDegreesToRadians(mWorldYaw-90));
            this->moveBy(x_projection, y_projection);

        }
        else if (key == Qt::Key_W) {
            float x_projection = stepLin * cosf(qDegreesToRadians(mWorldYaw));
            float y_projection = stepLin * sinf(qDegreesToRadians(mWorldYaw));
            //qDebug() << "x+proj: " << x_projection << "y_proj" << y_projection;
            this->moveBy(x_projection, y_projection);
        }
        else if (key == Qt::Key_S) {

           float x_projection = stepLin * cosf(qDegreesToRadians(mWorldYaw));
           float y_projection = stepLin * sinf(qDegreesToRadians(mWorldYaw));
           //qDebug() << "x+proj: " << x_projection << "y_proj" << y_projection;
           this->moveBy(- x_projection, - y_projection);
        }
        else if (key == Qt::Key_A){
            mWorldYaw -= stepRot;
            truncateYaw();
            this->setRotation(mWorldYaw);
        }
        else if (key == Qt::Key_D){
            mWorldYaw += stepRot;
            truncateYaw();
            this->setRotation(mWorldYaw);
        }

        mSceneX = this->x();
        mSceneY = this->y();

        emit sendCoords(mSceneX, mSceneY, mWorldYaw);
        if (mDebug){
            qDebug() << "Нажатие: " << key << "x: " << mSceneX << "y" << mSceneY << "yaw: " <<mWorldYaw ;
        }

      //this->scene->update(0, 0, scene->width(), scene->height());
}
