#include "mainwindow.h"
//#include "su_rov.h"

#include <QApplication>





int main(int argc, char *argv[])
{

    //double res = qRadiansToDegrees(qAtan(1 / qFabs(0)));

    QApplication a(argc, argv);
    //SU_ROV su;

    MainWindow w;
    w.show();
    return a.exec();
}
