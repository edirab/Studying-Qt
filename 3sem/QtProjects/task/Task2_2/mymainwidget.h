#ifndef MYMAINWIDGET_H
#define MYMAINWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MyMainWidget;
}


class MyMainWidget : public QWidget
{
    Q_OBJECT

public:
    MyMainWidget(QWidget *parent = 0);
    ~MyMainWidget();

    Ui::MyMainWidget *ui;

public slots:
    void change_LCD(){
        //ui->
        return;
    }
};

#endif // MYMAINWIDGET_H
