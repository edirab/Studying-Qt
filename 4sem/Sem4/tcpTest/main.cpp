#include "widget.h"
#include "clientform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;//объект класса формы отправителя
    ClientForm cf;

    w.show();
    cf.show();

    return a.exec();
}
