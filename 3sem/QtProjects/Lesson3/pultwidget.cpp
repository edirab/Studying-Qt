#include "pultwidget.h"
#include <QFile>
#include <QWidget>
//#include <ui_form.h>

PultWidget::PultWidget(QFrame *parent)
    : QFrame(parent)
{
    //ui = new Ui::Form();
    //ui->setupUi(this);
    //setupUi(this);

    QUiLoader *puil = new QUiLoader(this);
    QFile file (":/form.ui");

    QFrame *pwgtForm = puil->load(&file,this);

    if (pwgtForm)
    {
        resize (pwgtForm.resize());
    }
}

PultWidget::~PultWidget()
{
    //delete ui;
}

