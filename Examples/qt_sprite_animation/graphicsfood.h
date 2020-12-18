#ifndef GRAPHICSFOOD_H
#define GRAPHICSFOOD_H

#include <QGraphicsEllipseItem>
#include <QObject>

class GraphicsFood : public QObject, public QGraphicsEllipseItem {
  Q_OBJECT
public:
  explicit GraphicsFood(QPointF pos, QObject *parent = 0);

private:
  const QSizeF Size = {6, 6};
};

#endif // GRAPHICSFOOD_H
