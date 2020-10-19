#include "graphicsfood.h"
#include <QBrush>

GraphicsFood::GraphicsFood(QPointF pos, QObject *parent)
  : QObject(parent), QGraphicsEllipseItem(pos.x(), pos.y(), 6, 6) {
  this->setBrush(QColor(0, 0, 255));
}
