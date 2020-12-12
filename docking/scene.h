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

    QVector<QVector<float>> data;
    QTimer *animTimer;

private:
    int animationStep = 5; // ms

public slots:
    void readFile();
    void AnimationStep();
    void startVisualization();

signals:
    void fileReadSuccessful();
    void fileReadFailed(QString);

};

#endif // SCENE_H
