#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>

#include <chargingstation.h>
#include <auv.h>
#include <viewingangle.h>


class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyScene(QWidget *parent = nullptr);

    void keyPressEvent(QKeyEvent *event) override;

    int mW, mH;

    QGraphicsItemGroup  *group_1;
    QGraphicsItemGroup  *group_2;

    AUV *auv;
    viewingAngle *vAngle;
    chargingStation *Station;

signals:

};

#endif // SCENE_H
