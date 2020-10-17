#include "sender.h"
#include "receiver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sender* w = new Sender();
    Receiver* r = new Receiver();

    //connect(w->Ui::Sender::ui->pushButton, &QPushButton::click, )

    w->show();
    r->show();
    return a.exec();
}
