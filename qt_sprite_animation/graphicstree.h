#ifndef GRAPHICSTREE_H
#define GRAPHICSTREE_H

#include <QGraphicsEllipseItem>
#include <QObject>

class GraphicsTree : public QObject, public QGraphicsEllipseItem {
  Q_OBJECT
public:
  explicit GraphicsTree(QRect rect, QObject *parent = 0);
};

#endif // GRAPHICSTREE_H
