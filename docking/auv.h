#ifndef AUV_H
#define AUV_H

#include <QObject>
#include <QGraphicsItem>

#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>
//#include <cmath>
#include <QtMath>

//using namespace std;

class AUV : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    bool auvDebug = false;

    // Экранные координаты
    int auv_x, auv_y;

    // Размеры в пикселях
    int auvDiam = 30;
    int auvLen = auvDiam * 4;

    QGraphicsItemGroup *group;
    QGraphicsScene *scene;

    // Реальные координаты в СК базовой станции
    float auvX, auvY;
    float auvYaw = 0;;

    // Ширина поля зрения камеры
    float angleView;

    explicit AUV(QObject *parent = nullptr);
    AUV(QGraphicsScene *s, QGraphicsItemGroup *g, QObject *parent);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    QRectF boundingRect() const override;

    void truncateYaw();
    void keyboardRedraw(QKeyEvent *event);
    void startDocking();

private:
    int stepLin = 6;
    int stepRot = 5;


signals:
    void sendCoords(int x, int y, int yaw);

};

#endif // AUV_H
