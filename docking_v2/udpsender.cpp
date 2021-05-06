#include "udpsender.h"


UdpSender::UdpSender(QObject *prt):QObject(prt)
{
    m_socket =  new QUdpSocket(this);

    qDebug() << m_socket->bind(QHostAddress::LocalHost, 13041);
    qDebug() << m_socket->errorString();

    m_receiveSocket = new QUdpSocket(this);
    qDebug()<<m_receiveSocket->bind(QHostAddress::LocalHost, 13044);
    qDebug()<<m_receiveSocket->errorString();

    connect(m_receiveSocket, SIGNAL(readyRead()), this, SLOT(readData()));
}


void UdpSender::send(double desired_yaw, double dir, double X_curr, double Z_curr){
    sendData.desired_yaw = desired_yaw;
    sendData.dir = dir;
    sendData.curr_X = X_curr;
    sendData.curr_Z = Z_curr;

    qDebug()<<m_socket->writeDatagram((char*)&sendData, sizeof(sendData), QHostAddress::LocalHost, 13042);
}


