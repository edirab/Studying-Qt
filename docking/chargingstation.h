#ifndef CHARGINGSTATION_H
#define CHARGINGSTATION_H

#include <QObject>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>

class chargingStation : public QObject
{
    Q_OBJECT
public:
    // Экранные координаты
    int screenX, screenY;

    // Размеры в пикселях
    int dockSize = 100;

    QGraphicsItemGroup *group;
    QGraphicsScene *scene;

    explicit chargingStation(QObject *parent = nullptr);
    chargingStation(QGraphicsScene *s, QGraphicsItemGroup *g, QObject *parent);

    void draw();

signals:

};

#endif // CHARGINGSTATION_H
