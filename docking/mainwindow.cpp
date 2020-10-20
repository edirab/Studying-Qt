#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
    Slot 1
*/
void MainWindow::receiveCoords(int x, int y, int yaw){
    //qDebug() << "received signal Coords";
    this->ui->lineEdit_X->setText(QString::number(x));
    this->ui->lineEdit_Y->setText(QString::number(y));
    this->ui->lineEdit_Yaw->setText(QString::number(yaw));
}


/*
    Slot 2
*/
void MainWindow::receiveViewingAngle(int angle){
    //qDebug() << "Received angle:" << angle;
    this->myScene->vAngle->mAngle = angle;
    this->ui->labelAngleView->setText(QString("Угол обзора камеры: " + QString::number(angle)));
    this->myScene->update(0, 0, width(), height()); // вызвать перерисовку сцены
}


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sceneW = this->ui->graphicsView->width();
    sceneH = this->ui->graphicsView->height();

    this->myScene = new MyScene(this->ui->graphicsView);
    //this->myScene->addRect(0, 0, 200, 100, QPen(Qt::black));

    this->timer = new QTimer();
    this->timer->setSingleShot(true);
    // Подключаем СЛОТ для отрисовки к таймеру
    //connect(timer, SIGNAL(timeout()), this, SLOT(slotAlarmTimer()));
    this->timer->start(50);

    this->ui->graphicsView->setScene(myScene);

    bool bOk = QObject::connect(myScene->auv, SIGNAL(sendCoords(int, int, int)), this, SLOT(receiveCoords(int, int, int)));
    //bool bOk = QObject::connect(myScene->auv, &AUV::sendCoords, this, &MainWindow::receiveCoords);
    bOk = bOk && QObject::connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::receiveViewingAngle);
    Q_ASSERT(bOk);
}


MainWindow::~MainWindow()
{
    delete ui;
}

