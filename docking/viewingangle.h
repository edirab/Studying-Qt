#ifndef VIEWINGANGLE_H
#define VIEWINGANGLE_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtMath>
#include <QDebug>

class viewingAngle : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:

    float angle;

    explicit viewingAngle(QObject *parent = nullptr);
    viewingAngle(QGraphicsScene *s, QGraphicsItemGroup *g, QObject *parent, int angle);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    QRectF boundingRect() const override;

signals:

};

#endif // VIEWINGANGLE_H
