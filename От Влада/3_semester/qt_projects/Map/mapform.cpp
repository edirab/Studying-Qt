#include "mapform.h"


MapForm::MapForm(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);

    hlay = new QHBoxLayout();

    splineSeries = new QSplineSeries ;
    scatterSeries = new QScatterSeries;

    //splineSeries->append(0, 6);
    //splineSeries->append(2, 4);

    chart = new QChart();//создать объект QChart
    chart->addSeries(splineSeries); //добавить данные на график
    chart->addSeries(scatterSeries);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    xAxis= new QValueAxis();
    yAxis= new QValueAxis();
    //установка максимального и минимального значения по оси (например +/- 100)
    xAxis->setRange(-100,100);
    //установка количества отметок (штрихов) на оси (по умолчанию =5)
    xAxis->setTickCount(11);
    //так устанавливается надпись для оси графика:
    xAxis->setTitleText("X, м");
    yAxis->setRange(-100,100);
    yAxis->setTickCount(11);
    yAxis->setTitleText("Y, м");

    chartView->chart()->setAxisX(xAxis, splineSeries);
    chartView->chart()->setAxisY(yAxis, splineSeries);
    chartView->chart()->setAxisX(xAxis, scatterSeries);
    chartView->chart()->setAxisY(yAxis, scatterSeries);

    chartView->chart()->legend();
    hlay->addWidget(chartView);
    setLayout(hlay);
}

MapForm::~MapForm() {
    delete hlay;
}

void MapForm::setXY(float X, float Y) {
    splineSeries->append(X,Y);
    scatterSeries->clear();
    scatterSeries->append(X,Y);
    //chart->scroll(X/100,Y/100);
}

void MapForm::moveRight(){
    chart->scroll(10,0);
}

void MapForm::moveLeft(){
    chart->scroll(-10,0);
}
void MapForm::scalePlus(){
    chart->zoomIn();
    chartView->chart()->zoom(2);
}

void MapForm::scaleMinus(){
    chart->zoomOut();
}
void MapForm::moveUp(){
    chartView->chart()->scroll(0,10);
}

void MapForm::moveDown(){
    chartView->chart()->scroll(0,-10);
}
void MapForm::clearChart(){


}
void MapForm::scaleReset(){
    chart->zoomReset();
}
