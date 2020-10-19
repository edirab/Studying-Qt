#include "scene.h"
#include "star.h"
#include "random_points.h"
#include <QGraphicsEllipseItem>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

Scene::Scene(QObject *parent)
  : QGraphicsScene(parent),
    m_activeItem(nullptr){
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsScene::mousePressEvent(event);

  int type = getBetween(1, 3);
  int radius = getBetween(10, 50);
  QPointF pos = event->scenePos();

  QColor color(getBetween(0,255), getBetween(0,255), getBetween(0,255));

  m_activeItem = nullptr;

  switch (type) {
  case 1:
    m_activeItem = new QGraphicsEllipseItem(-radius,-radius, radius*2, radius*2);
    static_cast<QGraphicsEllipseItem*>(m_activeItem)->setBrush(color);
    break;
  case 2:
    m_activeItem = new Star(radius, color, this);
    break;
  }

  if (nullptr == m_activeItem)
    return;

  addItem(m_activeItem);
  m_activeItem->setPos(pos);
}


void Scene::keyPressEvent(QKeyEvent *event) {
  QGraphicsScene::keyPressEvent(event);

  if (nullptr == m_activeItem)
    return;

  switch (event->key()) {
  case Qt::Key::Key_A:
    m_activeItem->moveBy(-5, 0);
    break;
  case Qt::Key::Key_S:
    m_activeItem->moveBy(0, 5);
    break;
  case Qt::Key::Key_D:
    m_activeItem->moveBy(5, 0);
    break;
  case Qt::Key::Key_W:
    m_activeItem->moveBy(0, -5);
    break;
  }
}

