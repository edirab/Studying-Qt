#include "scene.h"
#include <QApplication>
#include <QGraphicsView>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  Scene scene;
  QGraphicsView view(&scene);

  scene.setSceneRect(-10, -10, 110, 110);

  view.show();

  a.exec();
}
