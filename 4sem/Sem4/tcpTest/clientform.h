#ifndef CLIENTFORM_H
#define CLIENTFORM_H

#include "ui_clientform.h"
#include <QString>
#include <QTcpSocket>
#include <QHostAddress>


class ClientForm : public QWidget, Ui::ClientForm {
    Q_OBJECT

public:
    explicit ClientForm(QWidget *parent = 0);
    virtual ~ClientForm();

private:
    QTcpSocket *pTcpSocket;//указатель на объект сокета

public slots:
    void readyRead();
};
#endif // CLIENTFORM_H
