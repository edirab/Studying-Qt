#include "su_rov.h"

SU_ROV::SU_ROV(QObject *parent) : QObject(parent)
{
    K_Protocol = new Qkx_coeffs("protocols.conf", "ki");
    X_Protocol = new x_protocol("protocols.conf", "xi", X);

    timer_period = 0.01;
    timer.start(timer_period*1000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
    Jz = 323;
    Cwz1 = 1200;
    Cwz2 = 110;
    metaH = 0.1;
    m = 320;
    g = 9.81;
    resetModel();
}

void SU_ROV::tick()
{
    // X[1] угол дифферента
    // X[2] угловая скорость
    X[4][0]=K[1]; // X[4] заданный угол дифферента
    X[3][0] = K[2]*(K[1]/57.3-X[1][0])-K[3]*X[2][0]+1/Jz*metaH*m*g*sin(X[1][0]);
    runge(X[3][0]);
}

void SU_ROV::f_x_u(double uz)
{
    da[0] = a[1];
    da[1] = 1/(Jz)*(-Cwz1*a[1]-Cwz2*a[1]*fabs(a[1])-metaH*m*g*sin(a[1])+uz);
}

void SU_ROV::runge(double uz, double h)
{
    double deltaX[MODEL_N];

    for(int i=0; i<MODEL_N; i++) {
        deltaX[i] = 0;
    }
    f_x_u(uz);

    for(int i=0; i<MODEL_N; i++) {
        deltaX[i] += da[i];
        a[i] = x[i]+h*da[i]*0.5;
    }
    f_x_u(uz);

    for(int i=0; i<MODEL_N; i++) {
        deltaX[i] += 2*da[i];
        a[i] = x[i]+h*da[i]*0.5;
    }
    f_x_u(uz);

    for(int i=0; i<MODEL_N; i++) {
        deltaX[i] += 2*da[i];
        a[i] = x[i]+h*da[i];
    }
    f_x_u(uz);

    for(int i=0; i<MODEL_N; i++) {
        deltaX[i] += da[i];
        x[i] += (h/6)*deltaX[i];
    }

    X[1][0] = x[0]; // X[1] угол дифферента
    X[2][0] = x[1]; // X[2] угловая скорость
    X[5][0] = X[1][0]*57.3; // угол дифферента, град
    X[6][0] = X[2][0]*57.3; // угловая скорость, град в сек
}

void SU_ROV::resetModel()
{
    for(int i=0; i<MODEL_N; i++) {
            a[i] = 0;
            da[i] = 0;
            x[i] = 0;
        }
}
