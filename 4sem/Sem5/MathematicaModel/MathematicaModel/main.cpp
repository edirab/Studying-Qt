#include <QCoreApplication>
#include "su_rov.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SU_ROV su;

    return a.exec();
}
