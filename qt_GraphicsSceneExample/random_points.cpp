#include "random_points.h"

QPoint plus(QPoint pivot, QPoint delta) {
  return QPoint(pivot.x() + delta.x(), pivot.y() + delta.y());
}

int getBetween(int from, int to) {
  int delta = to-from;
  if (0 == delta)
    return from;
  int number = rand()%delta;
  return from + number;
}

QPoint getBetween(QPoint from, QPoint to) {
  return QPoint(getBetween(from.x(), to.x()),
                getBetween(from.y(), to.y()));
}

QPoint getNear(QPoint pivot, QPoint range) {
  int dx = getBetween(0, range.x());
  int dy = getBetween(0, range.y());
  return plus(pivot, QPoint(dx,dy));
}
