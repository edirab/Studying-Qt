#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include "ui_widget.h"


namespace BUTTON {
class Widget : public QWidget, public Ui::Widget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:


public slots:
    void btnNormalClicked();
};
} //namespace BUTTON
#endif // WIDGET_H
