#include "senderwidget.h"
#include "ui_senderwidget.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

SenderWidget::SenderWidget(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);
    //прочитаем конфигурационные данные из файла
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
        ip.setAddress(obj.value("sender.ip").toString());
        port=obj.value("sender.port").toInt();
        //считаем IP адрес и порт получателя файла:
        receiverIP.setAddress(obj.value("receiver.ip").toString());
        receiverPort=obj.value("receiver.port").toInt();
        //выведем полученные значения в окно приложения
        lblReceiverIPport->setText(receiverIP.toString()+":"+QString::number(receiverPort));
        lblSenderIPport->setText(ip.toString()+":"+QString::number(port));

        udp = new QUdpSocket();
        udp->bind(ip, port);
        PATH = "./config.json";
        sent = 0;
    }
    else {
        qDebug()<<"can't open config.file";
    }


}

//синтаксис функции таков, что сигнал clicked() кнопки
//tbtnSend автоматически соединен с нижеследующим слотом
void SenderWidget::on_tbtnSend_clicked() {

    /*
    Dialog *dial= new Dialog();
    dial->show();
    connect(dial,SIGNAL(ok(QString)),
            this, SLOT(receivePath(QString)));*/
    QByteArray baDatagram;
    QFile file(PATH);

    if (!file.exists()) qDebug() << "File does not exist";
    else{

        file.open(QFile::ReadOnly);
        baDatagram = file.readAll();
        sent += udp->writeDatagram(baDatagram, receiverIP, receiverPort);
        qDebug() << "Sent: " << sent << " bytes";
        lblSended->setText(QString::number(sent));
    }
}

void SenderWidget::on_tbtnOpen_clicked() {

    Dialog *dial = new Dialog(this);
    dial->show();

    connect(dial, SIGNAL(ok(QString)),
            this, SLOT(receivePath(QString)));
}

void SenderWidget::receivePath(const QString &path){
    qDebug() << path;
    PATH=path;
    QFile file(path);
    QByteArray buffer;
    if (!file.exists()){
        lblPath->setText(tr("The file doesn't exists"));
        qDebug()<<"The file doesn't exists";
    }
    else {
        file.open(QFile::ReadOnly);
        buffer=file.readAll();
        txtBrFile->setText(buffer);
        lblPath->setText(path);
    }
}

SenderWidget::~SenderWidget()
{

}
