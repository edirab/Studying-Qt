#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    int x;
    //connect(ui->progressBar, &QProgressBar, );
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->progressBar, &QProgressBar::setValue);
}

Widget::~Widget()
{

}

//Widget::ChangeProgressBar(){
//    //this->x =

//}
