#include "su_rov.h"

SU_ROV::SU_ROV(QObject *parent) : QObject(parent)
{
    plot_trajectory();

    psiDesired = 10;
    psiCurrent =0;
    K1=2;
    K2=1;
    dPsi =0;
    Upsi =0;
    connect(&timer, SIGNAL(timeout()), SLOT(tick()));
    timer.start(10);

}

void SU_ROV::polar_to_cartesian(double &x, double &y, float r, float theta){

    x = r * cos(theta);
    y = r * sin(theta);
    return;
}



void SU_ROV::plot_trajectory()
{

    QVector<double> x_outer;
    QVector<double> x_inner;
    QVector<double> y_outer;
    QVector<double> y_inner;

    constexpr float D_mass_center_max = 13;
    constexpr float D_mass_center_min = 5.8;

    double angle = 90;
    double angular_delta = 15;

    int N = 360 / angular_delta;

    for (int i = 0; i < N; i++){
        double p1_real = 0;
        double p1_img = 0;

        double p2_real = 0;
        double p2_img = 0;

        polar_to_cartesian(p1_real, p1_img, D_mass_center_max, qDegreesToRadians(angle));
        polar_to_cartesian(p2_real, p2_img, D_mass_center_min, qDegreesToRadians(angle + angular_delta/2));

        x_outer.push_back(p1_real);
        y_outer.push_back(p1_img);

        x_inner.push_back(p2_real);
        y_inner.push_back(p2_img);

        angle += angular_delta;
    }

    //debug
    bool debug = false;
    if (debug){

        QDebug deb = qDebug();

        for (double d : x_outer) deb << d << " ";
        deb << "\n\n";
        for (double d : x_inner) deb << d << " ";
        deb << "\n\n";
        for (double d : y_outer) deb << d << " ";
        deb << "\n\n";
        for (double d : y_inner) deb << d << " ";
        deb << "\n\n";
    }

    for (int i = 0; i < x_outer.size(); i++){
        this->x_final.push_back((x_outer[i]));
        this->x_final.push_back((x_inner[i]));
        this->direction.push_back("forward");

        this->y_final.push_back(y_outer[i]);
        this->y_final.push_back(y_inner[i]);
        this->direction.push_back("backward");
    }
}


void SU_ROV::tick()
{
    dPsi = udp.getData().dPsi;
    psiCurrent = udp.getData().Psi;
    Upsi = (psiDesired - psiCurrent)*K1 - K2*dPsi;
    //Upsi = 10;
    udp.send(Upsi);
}
