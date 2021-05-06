#ifndef WIDGET_H
#define WIDGET_H

#include "ui_widget.h"

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>


class Widget : public QWidget, Ui::Widget {
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    virtual ~Widget();

    QTcpServer *pTcpServer;
    QTcpSocket *pTcpSocket;

public slots:
    void processingRequest();
    void readyRead();

private:

};

#endif // WIDGET_H
