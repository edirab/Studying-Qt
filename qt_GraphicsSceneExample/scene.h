#ifndef SCENE_H
#define SCENE_H
#include <QGraphicsScene>

class Scene : public QGraphicsScene {
  Q_OBJECT
public:
  explicit Scene(QObject *parent = nullptr);
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

  QGraphicsItem *m_activeItem;

};

#endif // SCENE_H
