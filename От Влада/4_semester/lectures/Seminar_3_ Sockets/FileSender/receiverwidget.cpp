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

}



ReceiverWidget::~ReceiverWidget()
{

}
