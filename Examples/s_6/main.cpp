#include "compassform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CompassForm w;
    w.setWindowTitle("Compass");
    w.show();
    return a.exec();
}
