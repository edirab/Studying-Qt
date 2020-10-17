#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_widget.h"

Ui::Widget::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
