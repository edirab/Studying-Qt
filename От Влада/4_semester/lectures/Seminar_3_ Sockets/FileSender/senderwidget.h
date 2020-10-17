#ifndef SENDERWIDGET_H
#define SENDERWIDGET_H

#include <QString>
#include <QWidget>
#include <QUdpSocket>
#include "ui_senderwidget.h"
#include "dialog.h"

class SenderWidget : public QWidget, Ui::SenderWidget {
    Q_OBJECT
public:
    explicit SenderWidget(QWidget *parent = 0);
    ~SenderWidget();
private slots:
    void on_tbtnOpen_clicked();
    void receivePath(const QString &path);
    void on_tbtnSend_clicked(); //слот, который по нажатию кнопки
    //отправит файл получателю
private:
    QUdpSocket *udp;//указатель на объект сокета
    QString PATH;//путь к файлу (прошлое занятие)
    QHostAddress ip, receiverIP;//переменные для работы с IP-адресами
    int port, receiverPort;//номера портов отправителя и получателя
    int sended; //переменная для хранения количества отправленных байт
};
#endif // SENDERWIDGET_H
