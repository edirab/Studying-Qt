#ifndef UDPSENDER_H
#define UDPSENDER_H
#include <QUdpSocket>



class UdpSender:public QObject
{
    Q_OBJECT
public:
    UdpSender(QObject *prt = nullptr);
private:

public slots:

public:

};

#endif // UDPSENDER_H
