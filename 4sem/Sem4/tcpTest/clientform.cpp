
#include "clientform.h"


ClientForm::ClientForm(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);
    setWindowTitle("Клиент");

    // 1
    pTcpSocket = new QTcpSocket(this);

    // 2
    if(pTcpSocket->bind(QHostAddress::LocalHost))
        lblSocketState->setText("binded");

    // 3
    connect(btnReceiveFile, &QToolButton::clicked, [=](){
        pTcpSocket->connectToHost(QHostAddress::LocalHost, 13303);
    });

    // 4
    connect(pTcpSocket, &QTcpSocket::connected, this, [&](){
        lblSocketState->setText("connected");
        lblFileName->setText("ui_clientform.h");

        QByteArray ba("ui_clientform.h");
        pTcpSocket->write(ba);
        lblExchange->setText("Filename sent");
    });

    connect(pTcpSocket, SIGNAL(readyRead()), SLOT(readyRead()));
}


void ClientForm::readyRead(){

    textBrowser->setText(pTcpSocket->readAll());
    lblExchange->setText("File received");
    pTcpSocket->close();
}


ClientForm::~ClientForm()
{
    pTcpSocket->close();
}
