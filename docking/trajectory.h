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

class Trajectory : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:

    QGraphicsScene *scene;
    QVector<QVector<float>> data;

    Trajectory(QGraphicsScene *s, QObject *parent);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    QRectF boundingRect() const override;

public slots:
    void getAnimIteration(int, int);
};

#endif // TRAJECTORY_H
