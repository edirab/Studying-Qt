#include "widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent)

{
    dt=5;
    V=0; psi=0; X=0; Y=0;
    time = new QTimer (this);
    setupUi(this);
    connect(btnStartTimer,SIGNAL(clicked(bool)),SLOT(startTimer(bool)));
    connect(btnClear,SIGNAL(clicked(bool)),wgtMap,SLOT(scaleReset()));
    connect(btnDown,SIGNAL(clicked(bool)),wgtMap,SLOT(moveDown()));
    connect(btnUp,SIGNAL(clicked(bool)),wgtMap,SLOT(moveUp()));
    connect(btnLeft,SIGNAL(clicked(bool)),wgtMap,SLOT(moveLeft()));
    connect(btnRight,SIGNAL(clicked(bool)),wgtMap,SLOT(moveRight()));
    connect(time,SIGNAL(timeout()), SLOT(tick()));
    connect(this, SIGNAL(setXY(float,float)),wgtMap,SLOT(setXY(float,float)));
}

Widget::~Widget()
{

}

void Widget::startTimer(bool status)
{
    V = edtV->text().toFloat();
    psi = edtPsi->text().toFloat();
    if (status){
        V=edtV->text().toFloat();
        psi=edtPsi->text().toFloat();
        time->start(dt);
    }
    else time->stop();

}

void Widget::tick()
{
    X+=dt*0.01*V*cos(psi/57.3);
    Y+=dt*0.01*V*sin(psi/57.3);
    emit setXY(X,Y);
    qDebug()<<X<<" "<<Y;
}
