#include "application.h"
#include "mainwindow.h"

int main (int argc, char *argv[]){
  
  HELLO::Application app (argc, argv);
  
  HELLO::MainWindow win;
  win.show();
  
  return app.exec();
}