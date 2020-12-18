#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->open, SIGNAL(clicked(bool)), SLOT(load_file()));
}

void MainWindow::load_file() {
  auto path = QFileDialog::getOpenFileName(
        this, "Open Dialog", "", "*.png *.jpg *.bmp *.JPG");
  if (path.isEmpty())
    return;
  QPixmap pixmap(path);
  ui->image->setScaledPixmap(pixmap);
}

MainWindow::~MainWindow() {
  delete ui;
}
