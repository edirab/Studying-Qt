#include "receiverwidget.h"
#include "ui_receiverwidget.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkDatagram>

ReceiverWidget::ReceiverWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    QFile configFile ("config.json");
    QJsonDocument jsonDoc;
    if (configFile.exists()){ //проверяем существование файла
        //если файл существует, то открываем его для чтения и работаем
        configFile.open(QFile::ReadOnly);
        //считаем конфигурационные данные из файла
        //используя метод fromJson, который преобразует текст (QByteArray)
        //в формат JSON
        jsonDoc = QJsonDocument().fromJson(configFile.readAll());
        configFile.close();
        //создадим JSON-объект, в который считаем содержимое jsonDoc
        QJsonObject obj = jsonDoc.object();
        //считаем из объекта IP адрес и порт отправителя файла:
        senderIp.setAddress(obj.value("sender.ip").toString());
        senderPort=obj.value("sender.port").toInt();
        //считаем IP адрес и порт получателя файла:
        receiverIp.setAddress(obj.value("receiver.ip").toString());
        receiverPort=obj.value("receiver.port").toInt();
        //выведем полученные значения в окно приложения
        lblReceiverIPport->setText(receiverIp.toString()+":"+QString::number(receiverPort));
        lblSenderIPport->setText(senderIp.toString()+":"+QString::number(senderPort));

        udpSocket = new QUdpSocket();
        udpSocket->bind(receiverIp, receiverPort);
        qDebug()<<"ip "<<receiverIp<<" port: "<<receiverPort<<" "<<udpSocket->errorString();
        //PATH = "./config.json";
        //sent = 0;
    }
    else {
        qDebug()<<"can't open config.file";
    }

    //udpSocket = new QUdpSocket(this);
    //udpSocket->bind(receiverIp, receiverPort);

    QObject::connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(socketReceived()));

}

void ReceiverWidget::socketReceived(){

    QNetworkDatagram networkDatagram;
    do{
        networkDatagram = udpSocket->receiveDatagram();
    }
    while( udpSocket -> hasPendingDatagrams());

    txtBrRec->setText(networkDatagram.data());
    senderIp = networkDatagram.senderAddress();
    senderPort = networkDatagram.senderPort();
    lblReceiverIPport->setText(senderIp.toString() + ":" + QString::number(senderPort));
    lblReceived->setText("Received: " + QString::number(networkDatagram.data().size()));

}


ReceiverWidget::~ReceiverWidget()
{

}
