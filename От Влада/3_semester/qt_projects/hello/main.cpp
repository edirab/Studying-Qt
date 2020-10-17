#include "application.h"
#include "mainwindow.h"

int main(int args, char* argv[]) {

    Application app(args,argv); //create application

    MainWindow w; //create window
    w.show();  //на этом месте возникала ошибка SEGMENTATION FAULT
    // путем недолгого гугления, ответ нашелся на stack overflow ()

    //ошибка возникала из-за того, что argc передавалась в конструктор Application не по ссылке, а напрямую
    return app.exec();
}
