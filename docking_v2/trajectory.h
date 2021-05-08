#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <parameters.h>
#include <QObject>
#include <QDebug>

#include <QKeyEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>

struct Coords{
    float timePoint{0};
    float X;
    float Z;
    float Yaw;
};



class Trajectory : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:

    QGraphicsScene *scene;
    QList<Coords> data;
    //int &animIteration_ref;
    int *animIteration_ptr = nullptr;

    Trajectory(QGraphicsScene *s, QObject *parent, int *animIteration);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    QRectF boundingRect() const override;

public slots:
    void receiveComputedCoords(float X, float Z, float Yaw);
};

#endif // TRAJECTORY_H
