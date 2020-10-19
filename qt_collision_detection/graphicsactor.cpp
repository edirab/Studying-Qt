#include "graphicsactor.h"
#include "animations_enum.h"
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <QDebug>
#include <QGraphicsScene>
#include <graphicstree.h>
#include <graphicsfood.h>

GraphicsActor::GraphicsActor(QPointF pos, QObject *parent)
  : AnimatedGraphicsItem(parent),
    m_currectAction(ActorActions::Stay),
    m_moveAnimationTimer(nullptr), m_posBeforeCollision(pos) {
  setScale(0.3);
  setSprites(m_currectAction, true);
  setPos(pos);
}

void GraphicsActor::setSprites(ActorActions sprites, bool force) {
  if (force || sprites != m_currectAction) {
    animation(sprites, Mode::Loop, true);
  }
}

void GraphicsActor::processKey(ActorActions action) {
  // sprites:
  if ((m_currectAction == ActorActions::Left && action == ActorActions::Right) ||
    (m_currectAction == ActorActions::Right && action == ActorActions::Left) ||
    (m_currectAction == ActorActions::Up && action == ActorActions::Down) ||
    (m_currectAction == ActorActions::Down && action == ActorActions::Up)) {
    action = ActorActions::Stay;
  }
  setSprites(action);

  // moving
  if (m_moveAnimationTimer) {
    m_moveAnimationTimer->stop();
    delete m_moveAnimationTimer;
    m_moveAnimationTimer = nullptr;
  }
  if (action != ActorActions::Stay) {
    m_moveAnimationTimer = new QTimeLine(AnimationPeriodMS, this);
    auto moveAnimation = new QGraphicsItemAnimation(m_moveAnimationTimer);

    connect(m_moveAnimationTimer, SIGNAL(finished()),
            this, SLOT(onAnimationFinished()));
    connect(m_moveAnimationTimer, SIGNAL(valueChanged(qreal)),
            this, SIGNAL(moved()));

    moveAnimation->setItem(this);
    moveAnimation->setTimeLine(m_moveAnimationTimer);

    switch (action) {
    case ActorActions::Right:
      moveAnimation->setPosAt(1.0, pos() + QPointF(SpeedPx, 0));
      break;
    case ActorActions::Left:
      moveAnimation->setPosAt(1.0, pos() + QPointF(-SpeedPx, 0));
      break;
    case ActorActions::Up:
      moveAnimation->setPosAt(1.0, pos() + QPointF(0, -SpeedPx));
      break;
    case ActorActions::Down:
      moveAnimation->setPosAt(1.0, pos() + QPointF(0, SpeedPx));
      break;
    }
    m_moveAnimationTimer->start();
  }

  m_currectAction = action;
}

void GraphicsActor::onAnimationFinished() {
  QTimeLine* sender_timeline = dynamic_cast<QTimeLine*>(sender());
  if (nullptr != sender_timeline) {
    sender_timeline->stop();
    if (m_moveAnimationTimer == sender_timeline)
      m_moveAnimationTimer = nullptr;
    delete sender_timeline;
  }
  processKey(m_currectAction);
}

bool GraphicsActor::processCollidings(QList<QGraphicsItem *> collidins) {
  bool can_move = true;
  for (QGraphicsItem* item: collidins) {
    if (dynamic_cast<GraphicsTree*> (item)) {
      can_move = false;
    }
    else if (dynamic_cast<GraphicsFood*> (item)) {
      static_cast<GraphicsFood*>(item)->deleteLater();
    }
  }
  return can_move;
}

void GraphicsActor::paint(QPainter* painter,
                          const QStyleOptionGraphicsItem* option,
                          QWidget* widget) {
  QPointF currentPos = pos();
  QList<QGraphicsItem *> colliding = scene()->collidingItems(this);

  if (processCollidings(colliding) == false) {
    if (m_moveAnimationTimer) {
      m_moveAnimationTimer->stop();
      delete m_moveAnimationTimer;
      m_moveAnimationTimer = nullptr;
    }
    setSprites(ActorActions::Stay);
    m_currectAction = ActorActions::Stay;

    setPos(m_posBeforeCollision);
  }
  else {
    m_posBeforeCollision = currentPos;
  }
  AnimatedGraphicsItem::paint(painter, option, widget);
}
