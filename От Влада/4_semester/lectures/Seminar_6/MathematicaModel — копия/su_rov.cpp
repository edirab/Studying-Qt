#include "su_rov.h"
#include <math.h>

SU_ROV::SU_ROV(QObject *parent) : QObject(parent)
{

    T=0.01;


}



float SU_ROV::saturation(float input, float max) {
    if (fabs(input)>= max)
        return (sign(input)*max);
    else return input;
}









