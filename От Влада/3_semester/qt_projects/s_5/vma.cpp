#include "vma.h"
#include "qmath.h"

vma::vma(QWidget *parent)
    : QFrame(parent)
{
    setupUi(this);
    barVPL->setStyleSheet("QProgressBar::chunk {background-color: red;}");
    barVPPr->setStyleSheet("QProgressBar::chunk {background-color: red;}");
    barVPL->setValue(0);
    barVPPr->setValue(0);
    marchValue = 0;

    connect(slVMAvalue, SIGNAL(valueChanged(int)), this, SLOT(slVMAvalueChanged(int)));
}

vma::~vma()
{
}

void vma::slVMAvalueChanged(int value)
{
    if (value < 0){
        barVPL->setStyleSheet("QProgressBar::chunk {background-color: green;}");
        barVPPr->setStyleSheet("QProgressBar::chunk {background-color: green;}");
        value = abs(value);
        barVPL->setValue(value);
        barVPPr->setValue(value);
    }
    else {
        barVPL->setStyleSheet("QProgressBar::chunk {background-color: red;}");
        barVPPr->setStyleSheet("QProgressBar::chunk {background-color: red;}");
        barVPL->setValue(value);
        barVPPr->setValue(value);
    }
}

