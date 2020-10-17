#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

namespace HELLO{
  class Application: public QApplication {
    Q_OBJECT 
    
  public:
    Application (int &argc, char* argv[]);
    
    virtual ~Application();
    
  };
  
}
#endif 