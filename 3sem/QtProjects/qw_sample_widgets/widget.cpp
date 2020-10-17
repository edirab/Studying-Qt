#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setMarshValue=0;
    //this->setSizePolicy(QSizePolicy::Minimum);

    lblMarsh = new QLabel(tr("Марш"));
    btnMarshPlus = new QPushButton(tr("+"), this);
    btnMarshMinus = new QPushButton(tr("-"), this);
    btnMarshReset = new QPushButton(QString::number(setMarshValue), this);

    btnMarshMinus->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    btnMarshPlus->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    btnMarshReset->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    hMarshLayout = new QHBoxLayout();
    vMarshLayout = new QVBoxLayout();

    vMarshLayout->addWidget(lblMarsh, 0, Qt::AlignCenter);
    hMarshLayout->addWidget(btnMarshMinus);
    hMarshLayout->addWidget(btnMarshPlus);

    vMarshLayout->addLayout(hMarshLayout);
    vMarshLayout->addStretch();
    vMarshLayout->addWidget(btnMarshReset);


    this->setLayout(vMarshLayout);

//    gMarshLayout = new QGridLayout;

//    gMarshLayout->addWidget(lblMarsh, 0, 0,1,2, Qt::AlignCenter);
//    gMarshLayout->addWidget(btnMarshPlus,1,0);
//    gMarshLayout->addWidget(btnMarshMinus,1,1);
//    gMarshLayout->addWidget(btnMarshReset,2,0,1,2);
//    this->setLayout(gMarshLayout);
}

Widget::~Widget()
{

}
