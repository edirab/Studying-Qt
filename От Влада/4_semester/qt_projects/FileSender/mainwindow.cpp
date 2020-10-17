#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pathdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->tbtnOpen, SIGNAL(clicked()), this, SLOT(openDial()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openDial()
{
    PathDialog *dial = new PathDialog(this);
    dial->show();
}
