#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "graphicsactor.h"

class View : public QGraphicsView {
  Q_OBJECT
public:
  explicit View(QSize frameSize, QWidget *parent = 0);
  void setFrameWithActor(QSize size);
public slots:
  QRect setActorFrame();
protected slots:
  void keyPressEvent(QKeyEvent *event);
  void resizeEvent(QResizeEvent *event);
private:
  QGraphicsScene m_scene;
  GraphicsActor m_actor;
  QPoint m_frameSize;
};

#endif // VIEW_H
