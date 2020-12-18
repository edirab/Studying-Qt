#ifndef CHARTFORM_H
#define CHARTFORM_H

#include <QWidget>
#include "ui_chartform.h"
#include <QtCharts>

class ChartForm : public QWidget, Ui::ChartForm
{
    Q_OBJECT

public:
    explicit ChartForm(QWidget *parent = 0);
    ~ChartForm();

private:
    QHBoxLayout *hlay;
    QChartView * chartView;
    QChart *chart;
    QLineSeries *series;
    QValueAxis *xAxis;
    QValueAxis *yAxis;
    QTimer *timer;

public slots:
    void setYT(double Y, int T);
};

#endif // CHARTFORM_H
