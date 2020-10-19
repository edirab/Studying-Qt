    #ifndef GRAPHICSACTOR_H
  #define GRAPHICSACTOR_H

  #include "animatedgraphicsitem.h"
  #include "animations_enum.h"

  class QGraphicsItemAnimation;
  class QTimeLine;

  class GraphicsActor : public AnimatedGraphicsItem {
    Q_OBJECT
  public:
    explicit GraphicsActor(QPointF pos, QObject *parent = 0);
  signals:
    void moved();
  public slots:
    void processKey(ActorActions key);
    void setSprites(ActorActions sprites, bool force = false);

    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = 0) override;

    void onAnimationFinished();

    bool processCollidings(QList<QGraphicsItem *> collidins);
  protected:
    const int AnimationPeriodMS = 1000;
    const int SpeedPx = 50;
    ActorActions m_currectAction;
    QTimeLine *m_moveAnimationTimer;

    QPointF m_posBeforeCollision;
  };

  #endif // GRAPHICSACTOR_H
