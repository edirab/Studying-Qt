#include "senderwidget.h"
#include "receiverwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SenderWidget w;//объект класса формы отправителя
    ReceiverWidget r;

    w.show();
    r.show();

    return a.exec();
}
