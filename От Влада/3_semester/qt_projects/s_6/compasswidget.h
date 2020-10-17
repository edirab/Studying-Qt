#ifndef COMPASSWIDGET_H
#define COMPASSWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>

#include "ui_compasswidget.h"

class CompassWidget : public QWidget, Ui::CompassWidget
{
    Q_OBJECT
public:
    explicit CompassWidget(QWidget *parent = nullptr);
    ~CompassWidget();

private:
    Ui::CompassWidget *ui;
    double currentYaw, setYaw;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *picROV;
    QGraphicsPixmapItem *picDial;
    QGraphicsTextItem *txtCurrentYaw;

signals:

public slots:
    void increaseCurrentYaw();
    void decreaseCurrentYaw();
    void resetCurrentYaw();
};

#endif // COMPASSWIDGET_H
