#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    QHBoxLayout* hMarshLayout;
    QVBoxLayout *vMarshLayout;
    QGridLayout *gMarshLayout;
    virtual ~Widget();

private:
    QPushButton *btnMarshMinus;
    QPushButton *btnMarshPlus;
    QPushButton *btnMarshReset;
    QLabel *lblMarsh;
    float setMarshValue;
};

#endif // WIDGET_H
