#include "pultwidget.h"

#include <QApplication>
#include <ui_form.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*QWidget *w = new QWidget();
    Ui::Form ui;
    ui.setupUi(w);
    w->show();*/
    PultWidget w;
    w.show();
    return a.exec();
}
