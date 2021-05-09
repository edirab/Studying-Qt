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

    double Vfwd = 1.3; // м/с
    double Rmin = 0.4;
    double Z_delay = 0.01;
};


class SU_ROV : public QObject
{
    Q_OBJECT
public:
    explicit SU_ROV(QObject *parent = 0);

    QTimer timer;
    int timer_period{10};

signals:
    void sendComputedCoords(float X, float Z, float Yaw);

public slots:
    void tick();

private:

    ModelParams modelParams;

    QVector<double> z_final;
    QVector<double> x_final;
    //QVector<QString> direction;


    UdpSender udp;

    void polar_to_cartesian(double &x, double &y, float r, float theta);
    void generate_path_points();

    void calc_dir();
    void check_end_simulation();
    void calc_desired_yaw();
    void constrain_yaw();
    void check_distance();

    void calc_position();
    double saturation_block(double value, double upper_lim = 24, double lower_lim = -24);

    bool received_packet;

    // ********* Рассчитывем в коде ***************
    double X_current{15};
    double Z_current{0};

    double desired_yaw{180};
    int dot_number{0};

    double X1{13}; // На 2 м выше самой первой точки
    double Z1{0};

    // ********* Отправляем **********************
    double deflection_yaw_constrained{180};
    double dir{1}; // 1 - fwd, -1 - backward, 0 - stop sim

    double U_fwd{0}; // На выходе регуляторов k1_m, k1_yaw
    double U_yaw{0};

    double U_bfs_yaw_1{0};

    // ********* Получаем *************************
    double real_yaw_vel{0};
    double real_yaw{180};
    double real_V_fwd{0.7}; // получаем по UDP

};

#endif // SU_ROV_H
