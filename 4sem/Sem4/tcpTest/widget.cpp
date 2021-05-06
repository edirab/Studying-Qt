#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    setWindowTitle("Сервер");

    pTcpServer = new QTcpServer(this);
    pTcpSocket = nullptr;

        if (pTcpServer->listen(QHostAddress::LocalHost, 13303))
            lblServerState->setText("Listen");
        else lblServerState->setText("error");

        lblServerIP->setText(pTcpServer->serverAddress().toString());
        lblServerPort->setNum(pTcpServer->serverPort());

        connect(pTcpServer, &QTcpServer::newConnection, this, &Widget::processingRequest);


}


void Widget::processingRequest(){

    lblServerState->setText("connected");
    lblExchange->setText("porcess in progress");

    pTcpSocket = pTcpServer->nextPendingConnection();
    //QObject::connect(pTcpSocket, &Widget::readyRead,  &Widget::readyRead);
    QObject::connect(pTcpSocket, SIGNAL(readyRead()), SLOT(readyRead()));
    QObject::connect(pTcpSocket, &QTcpSocket::disconnected, pTcpSocket, &QTcpSocket::deleteLater);
}

void Widget::readyRead(){

        QByteArray ba;

        ba = pTcpSocket->readAll();
        lblExchange->setText("request to send file: " + ba);
        QFile file(ba);

        if (file.exists()){
            file.open(QFile::ReadOnly);
            lblFile->setText(ba);
            lblSize->setText(QString::number(file.size()));

            pTcpSocket->write(file.readAll());
            lblExchange->setText("File sent");
            file.close();
        }
        else{
            lblExchange->setText("File does not exist");
        }
}

Widget::~Widget()
{
    //pTcpSocket->close();
}
