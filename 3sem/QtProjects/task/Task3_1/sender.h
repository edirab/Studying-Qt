#ifndef SENDER_H
#define SENDER_H

//#include <QMainWindow>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui{
    class Sender;
}

class Sender : public QWidget
{
    Q_OBJECT

public:


    Sender(QWidget *parent = 0);
    ~Sender();

     Ui::Sender *ui;

};
#endif // SENDER_H
