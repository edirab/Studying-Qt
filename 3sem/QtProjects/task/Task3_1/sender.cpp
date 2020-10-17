#include "sender.h"
#include "ui_sender.h"

Sender::Sender(QWidget *parent)
    : QWidget(parent)//, ui(new Ui::Sender)
{
    ui = new Ui::Sender();
    ui->setupUi(this);



}

Sender::~Sender()
{

}

