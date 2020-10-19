#include "view.h"
#include <QKeyEvent>
#include "graphicstree.h"
#include "graphicsfood.h"

View::View(QSize frameSize, QWidget *parent)
  : QGraphicsView(parent), m_actor(QPointF(0, 0)),
    m_frameSize(frameSize.width(), frameSize.height()) {
  setScene(&m_scene);
  m_scene.addItem(&m_actor);

  m_scene.setSceneRect(-100, -100, 200, 200);
  setSceneRect(-100, -100, 100, 100);

  m_scene.addItem(new GraphicsTree(QRect(50, 80, 100, 100)));
  m_scene.addItem(new GraphicsTree(QRect(250, 60, 150, 150)));
  m_scene.addItem(new GraphicsTree(QRect(-60, -30, 50, 50)));

  for (int i = 0; i < 80; ++i)
    m_scene.addItem(new GraphicsFood(QPointF(qrand()%400 - 50,qrand()%400 - 50)));

  connect(&m_actor, SIGNAL(moved()), this, SLOT(setActorFrame()));
}

void View::keyPressEvent(QKeyEvent *event) {
  switch(event->key()) {
  case Qt::Key_Left:
    m_actor.processKey(ActorActions::Left);
    break;
  case Qt::Key_Down:
    m_actor.processKey(ActorActions::Down);
    break;
  case Qt::Key_Right:
    m_actor.processKey(ActorActions::Right);
    break;
  case Qt::Key_Up:
    m_actor.processKey(ActorActions::Up);
    break;
  }
}

QRect View::setActorFrame() {
  QPoint actorPos = m_actor.pos().toPoint();
  QRect actorRect = QRect(actorPos - m_frameSize, actorPos + m_frameSize);
  this->setSceneRect(actorRect);
  return actorRect;
}

void View::resizeEvent(QResizeEvent *event) {
  this->fitInView(setActorFrame(), Qt::KeepAspectRatio);
  QGraphicsView::resizeEvent(event);
}
