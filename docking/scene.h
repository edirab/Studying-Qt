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

    QVector<QVector<float>> data;
    QTimer *animTimer;
    int animIteration = 0;

private:
    int animationStep = 5; // ms

public slots:
    void readFile();
    void AnimationStep();
    void startAminTimer();
    void stopAnimTimer();
    void toggleTrajectory();
    void sliderMoved(int);


signals:
    void fileReadSuccessful();
    void fileReadFailed(QString);
    void sendCoordsDuringAnimation(float, float, float);
    void sendCurrentIterationStep(int, int);

};

#endif // SCENE_H
