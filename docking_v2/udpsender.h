#ifndef UDPSENDER_H
#define UDPSENDER_H
#include <QUdpSocket>

/*
    Поля передаются в том поряде, в каком они обхявлены в структуре !
*/
struct ToMatlab{

    ToMatlab():U_bfs_yaw_out(0), U_bfs_fwd_out(1), curr_X(0), curr_Z(0) {}

    double U_bfs_yaw_out;
    double U_bfs_fwd_out;

    // Для отладки в Матлабе.  Выводим на Scope'ы, сравниваем с полной моделью в Матлабе
    double curr_X;
    double curr_Z;

    double U_yaw{0};
    double U_fwd{0};

    double U_bfs_yaw_1{0};
    double U_bfs_fwd_1{0};

    double curr_sin_integral{0};


};


struct FromMatlab{
    FromMatlab(){
        real_yaw_vel = 0;
        real_yaw = 0;
        real_V = 0;
        real_sin = 0;

    }
    double real_yaw_vel;
    double real_yaw;
    double real_V;
    double real_sin;
};


class UdpSender:public QObject
{
    Q_OBJECT
public:
    UdpSender(QObject *prt = nullptr);
private:
    ToMatlab sendData;
    FromMatlab receivedData;

    QUdpSocket *m_socket;
    QUdpSocket *m_receiveSocket;
public slots:

    void send(double U_bfs_yaw_out,
              double U_bfs_fwd_out,
              // Для отладки
              double curr_X,
              double curr_Z,
              double U_yaw = 0,
              double U_fwd = 0,
              double U_bfs_yaw_1 = 0,
              double U_bfs_fwd_1 = 0,
              double curr_sin_integral = 0);

    void readData(){
        while(m_receiveSocket->hasPendingDatagrams()){
            m_receiveSocket->readDatagram((char*)&receivedData, sizeof(receivedData));
            //qDebug() <<"received X: " << receivedData.real_yaw;
            //qDebug() << "received Y: "<< receivedData.real_V;
        }
    }
public:
    FromMatlab getData(){ return receivedData; }

};

#endif // UDPSENDER_H
