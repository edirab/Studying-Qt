#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    Ui::Widget *ui;

//signals:
//    void SliderChanged();

//public slots:
//    void ChangeProgressBar();

};

#endif // WIDGET_H
