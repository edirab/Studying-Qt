#include "su_rov.h"

SU_ROV::SU_ROV(QObject *parent) : QObject(parent)
{
    psiDesired = 10;
    psiCurrent =0;
    K1=2;
    K2=1;
    dPsi =0;
    Upsi =0;

}

void SU_ROV::tick()
{

}
