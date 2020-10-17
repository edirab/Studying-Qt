#include "senderwidget.h"
#include "receiverwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SenderWidget w;//объект класса формы отправителя

    w.show();


    return a.exec();
}
