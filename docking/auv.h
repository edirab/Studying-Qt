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
    bool mDebug = false;

    int mSceneX, mSceneY; // Экранные координаты
    int mDiam = 30; // Размеры в пикселях
    int mLen = mDiam * 4;

    QGraphicsItemGroup *group;
    QGraphicsScene *scene;

    float mWorldX, mWorldY; // Реальные координаты в СК базовой станции
    float mWorldYaw = 0;

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
