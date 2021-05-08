#ifndef SCENE_H
#define SCENE_H

#include <QTimer>
#include <QThread>
#include <QObject>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>

#include <chargingstation.h>
#include <auv.h>
#include <trajectory.h>
#include <viewingangle.h>

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyScene(QWidget *parent = nullptr);

    void keyPressEvent(QKeyEvent *event) override;


    int mWidth, mHeight;

    QGraphicsItemGroup  *mGroupStation;
    QGraphicsItemGroup  *mGroupAUV;

    AUV *auv;

    viewingAngle *vAngle;
    chargingStation *Station;
    Trajectory *trajectory;

    QTimer *animTimer;
    int animIteration = 0;

private:
    int animationStep = 33; // ms

public slots:

    void AnimationStep();
    void startAminTimer();
    void stopAnimTimer();

signals:
    void sendCoordsDuringAnimation(float, float, float);
    void sendCurrentIterationStep(int, int);

};

#endif // SCENE_H
