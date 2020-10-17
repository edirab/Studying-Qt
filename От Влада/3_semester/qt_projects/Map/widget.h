#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "ui_widget.h"



class Widget : public QWidget, public Ui::Widget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void startTimer(bool);
    void tick();
signals:
    void setXY(float X, float Y);
private:
    int dt;
    QTimer *time;
    float X,Y,V,psi;


};

#endif // WIDGET_H
