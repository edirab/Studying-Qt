#ifndef UDPSENDER_H
#define UDPSENDER_H
#include <QUdpSocket>

struct ToMatlab{

    ToMatlab():Upsi(0){}
    double Upsi;
};


struct FromMatlab{
    FromMatlab(){
        Psi = 0;
        dPsi = 0;
    }
    double Psi;
    double dPsi;
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

    void send(double U);

    void readData(){
        while(m_receiveSocket->hasPendingDatagrams()){
            m_receiveSocket->readDatagram((char*)&receivedData, sizeof(receivedData));
            qDebug() <<"received X: " << receivedData.Psi;
            qDebug() << "received Y: "<< receivedData.dPsi;
        }
    }
public:
    FromMatlab getData(){ return receivedData; }

};

#endif // UDPSENDER_H
