#include "compassform.h"
#include "ui_compassform.h"

CompassForm::CompassForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CompassForm)
{
    ui->setupUi(this);
    closeFlag = 0;

    operatingTime = new QTime();
    operatingTime->start();
    prevOperatingTime = operatingTime->elapsed();

    date = new QDate();
    time = new QTime();
    timerWidgetID = this->startTimer(100);

    connect(ui->sldYawSet, SIGNAL(valueChanged(int)),ui->picFrame, SLOT(setYaw(int)));
    connect(ui->sldYawCurrent , SIGNAL(valueChanged(int)),ui->picFrame, SLOT(setYawDesirable(int)));
}

CompassForm::~CompassForm()
{
    delete ui;
}

void CompassForm::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == timerWidgetID)
    {
        *date =QDate::currentDate();
    }
    //else QWidget::QTimerEvent(e);
}

