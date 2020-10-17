#ifndef CLIENTFORM_H
#define CLIENTFORM_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class ClientForm;
}

class ClientForm : public QWidget
{
    Q_OBJECT

public:
    explicit ClientForm(QWidget *parent = nullptr);
    ~ClientForm();

public slots:
    void readyRead();

private:
    Ui::ClientForm *ui;
    QTcpSocket *pTcpSocket;
};

#endif // CLIENTFORM_H
