#include "chartform.h"


ChartForm::ChartForm(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    chart = new QChart();
    chartView = new QChartView(chart);
    series = new QLineSeries;
    xAxis = new QValueAxis();
    yAxis = new QValueAxis();

    chart->addSeries(series);
    chart->createDefaultAxes();

    //установка максимального и минимального значения по оси
    xAxis->setRange(0,100);
    //установка количества градаций на оси (по умолчанию =5)
    xAxis->setTickCount(11);
    xAxis->setTitleText("T, c");


    yAxis->setRange(0,100);
    yAxis->setTickCount(11);
    yAxis->setTitleText("dt, c");


    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->setAxisX(xAxis, series);
    chartView->chart()->setAxisY(yAxis, series);

    hlay = new QHBoxLayout();
    hlay->addWidget(chartView);
    setLayout(hlay);

}

ChartForm::~ChartForm()
{

}

void ChartForm::setYT(double Y, int T)
{
    series->append(T/1000, Y);

}

