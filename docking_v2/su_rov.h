#ifndef SU_ROV_H
#define SU_ROV_H

#include <QObject>
#include <QTimer>
#include <QtMath>
#include <QVector>
#include "udpsender.h"

struct HydroDyn{

    double Cvx1 = 182.87;
    double Cvx2 = 33;
    double Cwy1 = 1144.7;
    double Cwy2 = 22.52;
};

struct AUV_{

    double m = 300;
    double lambda_11 = 12.72;
    double Jyy = 366;
    double lambda_55 = 117.08;
    double P1 = 60 * 9.81;
    double U1 = 24;

    double K_thruster = P1 / U1;
    double T_thruster = 0.3;
    double l = 0.225;

    double k1_m = 4.5227;
    double k2_m = 5.32;

    double k1_yaw = 0.5146;
    double k2_yaw = 0.6370;

    double kc = 57.2958;


};


class ModelParams{

public:

    AUV_ auv;
    HydroDyn hDyn;

    double Rmin = 0.4;
    double Z_delay = 0.01;
};


class SU_ROV : public QObject
{
    Q_OBJECT
public:
    explicit SU_ROV(QObject *parent = 0);

signals:

public slots:
    void tick();

private:

    ModelParams modelParams;

    QVector<double> x_final;
    QVector<double> y_final;
    QVector<QString> direction;

    QTimer timer;
    UdpSender udp;

    void polar_to_cartesian(double &x, double &y, float r, float theta);
    void plot_trajectory();
    void stop_simulation();

    //заданный, текущий курс и угловая скорость по курсу
    double psiDesired,psiCurrent,dPsi;
    //коэффициенты K1,K2
    double K1,K2;
    //управляющий сигнал
    double Upsi;

    double X_current;
    double Z_current;
};

#endif // SU_ROV_H
