#ifndef RANDOM_POINTS_H
#define RANDOM_POINTS_H

#include <QPoint>

int getBetween(int from, int to);
QPoint getBetween(QPoint from, QPoint to);

QPoint getNear(QPoint pivot, QPoint range);

#endif // RANDOM_POINTS_H
