#ifndef AUV_H
#define AUV_H

#include <QObject>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>

class AUV : public QObject
{
    Q_OBJECT
public:
    bool auvDebug;

    // Экранные координаты
    int auv_x, auv_y;

    // Размеры в пикселях
    int auvDiam = 30;
    int auvLen = auvDiam * 4;

    QGraphicsItemGroup *group;
    QGraphicsScene *scene;

    // Реальные координаты в СК базовой станции
    float auvX, auvY;
    float auvYaw;

    // Ширина поля зрения камеры
    float angleView;

    explicit AUV(QObject *parent = nullptr);
    AUV(QGraphicsScene *s, QGraphicsItemGroup *g, QObject *parent);

    void draw();
    void startDocking();

signals:

};

#endif // AUV_H
