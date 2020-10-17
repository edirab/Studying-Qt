#include "mymainwidget.h"
#include "ui_mymainwidget.h"
#include <QDial>
#include <QLCDNumber>

MyMainWidget::MyMainWidget(QWidget *parent)
    : QWidget(parent),  ui(new Ui::MyMainWidget)
{

    ui->setupUi(this);
    //connect(ui->horizontalSlider, &QSlider::valueChanged, ui->progressBar, &QProgressBar::setValue);
    //connect(this->ui->dial, &QDial::valueChanged, this->ui->lcdNumber, &QLCDNumber::display);

    connect(ui->dial, SIGNAL(valueChanged(int)), ui->lcdNumber, SLOT(display(int)));
}

MyMainWidget::~MyMainWidget()
{

}
