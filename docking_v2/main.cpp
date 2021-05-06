#include "mainwindow.h"
#include "su_rov.h"

#include <QApplication>





int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    SU_ROV su;

    MainWindow w;
    w.show();
    return a.exec();
}
