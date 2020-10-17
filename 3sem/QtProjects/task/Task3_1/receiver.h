#ifndef RECEIVER_H
#define RECEIVER_H

#include <QWidget>
#include "ui_receiver.h"

QT_BEGIN_NAMESPACE
namespace Ui{
    class Receiver;
}

class Receiver : public QWidget
{
    Q_OBJECT
public:
    explicit Receiver(QWidget *parent = nullptr);
    //~Receiver();
    Ui::Receiver *ui;

signals:

};

#endif // RECEIVER_H
