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


void UdpSender::send(double U_bfs_yaw_out,
                     double U_bfs_fwd_out,
                     double X_curr,
                     double Z_curr,
                     double U_yaw,
                     double U_fwd,
                     double U_bfs_yaw_1,
                     double U_bfs_fwd_1,
                     double curr_sin_integral)
{
    sendData.U_bfs_yaw_out = U_bfs_yaw_out;
    sendData.U_bfs_fwd_out = U_bfs_fwd_out;
    sendData.curr_X = X_curr;
    sendData.curr_Z = Z_curr;

    sendData.U_yaw = U_yaw;
    sendData.U_fwd = U_fwd;

    sendData.U_bfs_yaw_1 = U_bfs_yaw_1;
    sendData.U_bfs_fwd_1 = U_bfs_fwd_1;

    // test
    sendData.curr_sin_integral = curr_sin_integral;

    qDebug()<<m_socket->writeDatagram((char*)&sendData, sizeof(sendData), QHostAddress::LocalHost, 13042);
}


