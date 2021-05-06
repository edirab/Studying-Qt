#ifndef SU_ROV_H
#define SU_ROV_H

#include <QObject>
#include <QTimer>
#include "udpsender.h"

class SU_ROV : public QObject
{
    Q_OBJECT
public:
    explicit SU_ROV(QObject *parent = 0);

signals:

public slots:
    void tick();

private:

    QTimer timer;
    UdpSender udp;
    //заданный, текущий курс и угловая скорость по курсу
    double psiDesired,psiCurrent,dPsi;
    //коэффициенты K1,K2
    double K1,K2;
    //управляющий сигнал
    double Upsi;
};

#endif // SU_ROV_H
