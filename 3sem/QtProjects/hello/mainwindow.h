#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace HELLO{
  class MainWindow: public QWidget{
    Q_OBJECT
   public:
     MainWindow(QWidget *parent = 0);
     
     virtual ~MainWindow();
     
  };
}
#endif