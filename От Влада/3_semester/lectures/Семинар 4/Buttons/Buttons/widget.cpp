#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QTransform>
namespace BUTTON {

Widget::Widget(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);
    this->setWindowTitle("Виды кнопок");
    btnRight->setIcon(QIcon(":/images/arrow.png"));
    btnRight->setIconSize(QSize(btnRight->width(),btnRight->height()));
    btnLeft->setIcon(QIcon(":/images/arrow_left.png"));
    btnLeft->setIconSize(QSize(btnLeft->width(),btnLeft->height()));

    //btnToggle->setCheckable(true);

    //при вызове этой функции с параметром true
    //кнопка становится плоской
    btnFlat->setFlat(true);


}

Widget::~Widget() {

}

void Widget::btnNormalClicked() {
    qDebug()<<"Clicked!";
}

} //namespace BUTTON
