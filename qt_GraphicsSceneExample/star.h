#ifndef STAR_H
#define STAR_H

#include <QGraphicsItem>
#include <QObject>

#define M_PI 3.1415926535897932482

class Star : public QObject, public QGraphicsItem {
  Q_OBJECT
public:
  Star(size_t radius, QColor color, QObject* parent);
protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) override;
  QRectF boundingRect() const override;
private:
  QPolygonF polygon() const;
protected:
  size_t m_radius;
  QColor m_color;
};


#endif // STAR_H
