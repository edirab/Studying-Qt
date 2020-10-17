#include "vma.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    vma w;
    w.show();
    return a.exec();
}
