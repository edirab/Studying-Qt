#include "clientform.h"
#include "ui_clientform.h"

ClientForm::ClientForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientForm)
{
    ui->setupUi(this);
    setWindowTitle("Client");
    pTcpSocket = new QTcpSocket(this);
    if (pTcpSocket->bind(QHostAddress::LocalHost)) ui->lblSocketState->setText("Binded");
    connect(ui->btnReceiveFile, &QToolButton::clicked, [=] (){
       pTcpSocket->connectToHost(QHostAddress::LocalHost, 13303);
    });
    connect(pTcpSocket, &QTcpSocket::connected, this, [&] (){
        ui->lblSocketState->setText("Connected");
        ui->lblFileName->setText("ui_clientform.h");
        QByteArray ba("ui_clientform.h");
        pTcpSocket->write(ba);
        ui->lblExchange->setText("Filename sended");
    });
    connect(pTcpSocket, SIGNAL(readyRead()), SLOT(readyRead()));
}

ClientForm::~ClientForm()
{
    delete ui;
}

void ClientForm::readyRead()
{
    ui->textBrowser->setText(pTcpSocket->readAll());
    ui->lblExchange->setText("File received");
    pTcpSocket->close();
}
