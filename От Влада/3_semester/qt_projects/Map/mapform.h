#ifndef MAPFORM_H
#define MAPFORM_H

#include <QWidget>
#include "ui_mapform.h"
#include <QtCharts>
#include <QHBoxLayout>

using namespace QtCharts;

class MapForm : public QWidget, public Ui::MapForm {
    Q_OBJECT
private:
    QChartView *chartView;
    QHBoxLayout *hlay;
    QChart *chart;
    QSplineSeries *splineSeries;
    QScatterSeries *scatterSeries;

    QValueAxis *xAxis;
    QValueAxis *yAxis;

public:
    explicit MapForm(QWidget *parent = 0);
    ~MapForm();


public slots:
    void setXY(float X,float Y);
    void moveDown();
    void moveUp();
    void scaleMinus();
    void scalePlus();
    void moveLeft();
    void moveRight();
    void clearChart();
    void scaleReset();
private:

};

#endif // MAPFORM_H
