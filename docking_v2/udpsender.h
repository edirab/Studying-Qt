#ifndef UDPSENDER_H
#define UDPSENDER_H
#include <QUdpSocket>

struct ToMatlab{

    ToMatlab():desired_yaw(0){}
    //double Upsi;
    double desired_yaw;
    double dir;
};


struct FromMatlab{
    FromMatlab(){
        real_yaw = 0;
        real_V = 0;
    }
    double real_yaw;
    double real_V;
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

    void send(double desired_yaw, double dir);

    void readData(){
        while(m_receiveSocket->hasPendingDatagrams()){
            m_receiveSocket->readDatagram((char*)&receivedData, sizeof(receivedData));
            qDebug() <<"received X: " << receivedData.real_yaw;
            qDebug() << "received Y: "<< receivedData.real_V;
        }
    }
public:
    FromMatlab getData(){ return receivedData; }

};

#endif // UDPSENDER_H
