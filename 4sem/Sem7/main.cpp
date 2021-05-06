#include <QApplication>
#include "su_rov.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SU_ROV su;
    return a.exec();
}
