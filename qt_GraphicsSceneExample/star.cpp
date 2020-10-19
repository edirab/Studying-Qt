#include "star.h"
#include <QPainter>
#include <cmath>

Star::Star(size_t radius, QColor color, QObject *parent)
  : QObject(parent), QGraphicsItem(),
    m_radius(radius), m_color(color) {
}

QRectF Star::boundingRect() const {
  return QRectF(QPoint(-m_radius, -m_radius), QPoint(m_radius, m_radius));
}

void Star::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) {
  painter->setPen(Qt::black);
  painter->setBrush(m_color);

  painter->drawPolygon(polygon());
}

QPolygonF Star::polygon() const {
  QPolygonF polygon;

  size_t small_radius = m_radius/2;

  double step = 360./5, angle_deg = 0;
  for (size_t count = 0; count < 5; ++count) {
    double angle_rad = angle_deg*M_PI/180;
    QPointF outerPoint(m_radius*cos(angle_rad), m_radius*sin(angle_rad));
    QPointF innerPoint(small_radius*cos(angle_rad), small_radius*sin(angle_rad));

    polygon << outerPoint << innerPoint;

    angle_deg += step;
  }

  return polygon;
}
