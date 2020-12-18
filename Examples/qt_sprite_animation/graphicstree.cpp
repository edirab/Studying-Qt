#include "graphicstree.h"
#include <QBrush>

GraphicsTree::GraphicsTree(QRect rect, QObject *parent)
  : QGraphicsEllipseItem(rect), QObject(parent) {
  this->setBrush(QColor(0, 255, 0));
}
