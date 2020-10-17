#ifndef RECEIVERWIDGET_H
#define RECEIVERWIDGET_H

#include <QWidget>
#include <QHostAddress>
#include <QUdpSocket>

#include "ui_receiverwidget.h"

class ReceiverWidget : public QWidget, Ui::ReceiverWidget {
    Q_OBJECT
public:
    explicit ReceiverWidget(QWidget *parent = 0);
    ~ReceiverWidget();
private:

};

#endif // RECEIVERWIDGET_H
