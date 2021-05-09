#include "su_rov.h"

SU_ROV::SU_ROV(QObject *parent) : QObject(parent)
{
    generate_path_points();

    connect(&timer, SIGNAL(timeout()), SLOT(tick()));
}

void SU_ROV::polar_to_cartesian(double &x, double &y, float r, float theta){

    x = r * cos(theta);
    y = r * sin(theta);
    return;
}

void SU_ROV::generate_path_points()
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
        this->z_final.push_back((x_outer[i]));
        this->z_final.push_back((x_inner[i]));
        //this->direction.push_back("forward");

        this->x_final.push_back(y_outer[i]);
        this->x_final.push_back(y_inner[i]);
        //this->direction.push_back("backward");
    }
}

void SU_ROV::calc_dir(){
    if (dot_number % 2 == 1){
        this->dir = 1; // fwd
    }else{
        this->dir = -1; // backward
    }

    /*
        NB: по началу нужно двигаться 2 раза вперёд:
        от (0, 15) к (0, 13) и
        далее к первой путевой точке
    */
    if (dot_number == 0){
        this->dir = 1;
    }
}

void SU_ROV::calc_desired_yaw(){

    double delta_x = this->X1 - this->X_current;
    double delta_z = this->Z1 - this->Z_current;

    // 1
    if (delta_x >= 0 && delta_z >= 0){
        this->desired_yaw = qRadiansToDegrees(atan(delta_z / delta_x));
    }
    // 2
    else if (delta_x >= 0 && delta_z < 0){
        this->desired_yaw = 360 - qRadiansToDegrees(atan(abs(delta_z) /  delta_x));

    }
    // 3
    else if (delta_x < 0 && delta_z < 0){
        this->desired_yaw = 180 + qRadiansToDegrees(atan(abs(delta_z)/ abs(delta_x)));
    }
    // 4
    else if (delta_x < 0 && delta_z >= 0) {
        this->desired_yaw = 180 - qRadiansToDegrees(atan(delta_z / abs(delta_x)));
    }

    if (dir == -1) {
        this->desired_yaw = this->desired_yaw - 180;
    }
}

void SU_ROV::check_distance(){
    double dst = sqrt( pow(Z_current - Z1, 2) + pow(X_current - X1, 2) );

    if (dst < modelParams.Rmin){
        this->dot_number++;
    }
}

void SU_ROV::check_end_simulation(){

    // Debug: test_1
//    if (X1 >= 13 && Z_current >= 12.5){
//        dir = 0;
//        this->timer.stop();
//    }

    if (Z_current >= 12.5){
        this->timer.stop();
    }
}

void SU_ROV::constrain_yaw(){

    if (abs(desired_yaw - real_yaw) > 180){

        if (desired_yaw > 180){
            deflection_yaw_constrained = desired_yaw - 360;
        } else {
            deflection_yaw_constrained = desired_yaw;
        }
    }
    else {
        deflection_yaw_constrained = desired_yaw;
    }

}

void SU_ROV::calc_position(){

    double L = real_V_fwd * timer_period / 1000 ; // ms

    double shortTravel_Z = L * sin(qDegreesToRadians(real_yaw));
    double shortTravel_X = L * cos(qDegreesToRadians(real_yaw));

    this->Z_current += shortTravel_Z;
    this->X_current += shortTravel_X;
    return;
}

double SU_ROV::saturation_block(double value, double upper_lim, double lower_lim){

    if (value > upper_lim){
        return upper_lim;
    } else if (value < lower_lim){
        return lower_lim;
    }else {
        return value;
    }

}

void SU_ROV::tick()
{
    // Получаем данные
    //dPsi = udp.getData().dPsi;
    //psiCurrent = udp.getData().Psi;
    this->real_yaw_vel = udp.getData().real_yaw_vel;
    this->real_yaw = udp.getData().real_yaw;
    this->real_V_fwd = udp.getData().real_V;

    // Вычисляем
    calc_position();
    check_distance();

    assert(dot_number < x_final.size());

    this->X1 = x_final[dot_number];
    this->Z1 = z_final[dot_number];

    calc_dir();
    check_end_simulation();
    calc_desired_yaw();
    constrain_yaw();

    this->U_fwd = this->dir * this->modelParams.Vfwd * this->modelParams.auv.k1_m;
    this->U_yaw = (this->deflection_yaw_constrained - this->real_yaw) * this->modelParams.auv.k1_yaw;

    this->U_bfs_yaw_1 = saturation_block(this->U_yaw - this->modelParams.auv.k2_yaw * qRadiansToDegrees(this->real_yaw_vel));
    this->U_bfs_fwd_1 = saturation_block(U_fwd - this->modelParams.auv.k2_m);

    this->U_bfs_yaw_out = saturation_block(this->U_bfs_yaw_1 + U_bfs_fwd_1);
    this->U_bfs_fwd_out = saturation_block(this->U_bfs_yaw_1 - U_bfs_fwd_1 );

    qDebug() << "Z_curr: " << Z_current
             << "X_curr: " << X_current
             << "Z_fin: " << z_final[dot_number]
             << "X_fin: " << x_final[dot_number]
             << "Des. yaw: " << desired_yaw
             << "Constr.: " << deflection_yaw_constrained
             << "Real yaw: " << real_yaw
             << "Real Vfwd: " << real_V_fwd <<  "\n";

    emit sendComputedCoords(X_current, Z_current, real_yaw); // to Trajectory class

    //udp.send(this->deflection_yaw_constrained, this->dir, this->X_current, this->Z_current);
    //udp.send(this->U_yaw, U_fwd, this->X_current, this->Z_current);
    //udp.send(this->U_bfs_yaw_1, U_bfs_fwd_1, this->X_current, this->Z_current);
    udp.send(this->U_bfs_yaw_out, U_bfs_fwd_out, this->X_current, this->Z_current);

}

















