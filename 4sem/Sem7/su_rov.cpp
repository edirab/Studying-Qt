#include "su_rov.h"

SU_ROV::SU_ROV(QObject *parent) : QObject(parent)
{
    psiDesired = 10;
    psiCurrent =0;
    K1=2;
    K2=1;
    dPsi =0;
    Upsi =0;
    connect(&timer, SIGNAL(timeout()), SLOT(tick()));
    timer.start(10);

}

void SU_ROV::tick()
{
    dPsi = udp.getData().dPsi;
    psiCurrent = udp.getData().Psi;
    Upsi = (psiDesired - psiCurrent)*K1 - K2*dPsi;
    //Upsi = 10;
    udp.send(Upsi);
}
