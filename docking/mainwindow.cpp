#include "mainwindow.h"
#include "ui_mainwindow.h"



void MainWindow::keyPressEvent(QKeyEvent *event) {
    auv->keyboardRedraw(event);
}

void MainWindow::receiveCoords(int x, int y, int yaw){
    //qDebug() << "received signal Coords";
    this->ui->lineEdit_X->setText(QString::number(x));
    this->ui->lineEdit_Y->setText(QString::number(y));
    this->ui->lineEdit_Yaw->setText(QString::number(yaw));
}


void MainWindow::receiveViewingAngle(int angle){
    //qDebug() << "Received angle:" << angle;
    this->vAngle->angle = angle;
    this->ui->labelAngleView->setText(QString("Угол обзора камеры: " + QString::number(angle)));
    // вызвать перерисовку сцены
    this->scene->update(0, 0, width(), height());
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    this->scene = new QGraphicsScene();   // Инициализируем сцену для отрисовки
    this->ui->graphicsView->setScene(scene);

    this->group_1 = new QGraphicsItemGroup(); // Инициализируем первую группу элементов
    this->group_2 = new QGraphicsItemGroup(); // Инициализируем вторую группу элементов

    this->scene->addItem(group_1);            // Добавляем первую группу в сцену
    this->scene->addItem(group_2);            // Добавляем вторую группу в сцену

    this->timer = new QTimer();               // Инициализируем Таймер
    this->timer->setSingleShot(true);
    // Подключаем СЛОТ для отрисовки к таймеру
    //connect(timer, SIGNAL(timeout()), this, SLOT(slotAlarmTimer()));
    this->timer->start(50);                   // Стартуем таймер на 50 миллисекунд

    /* Устанавливаем размер сцены по размеру виджета
     * Первая координата - это левый верхний угол,
     * а Вторая - это правый нижний угол
     * */

    this->sceneW = this->ui->graphicsView->width();
    this->sceneH = this->ui->graphicsView->height();
    this->scene->setSceneRect(0,0, sceneW, sceneH);

    Station = new chargingStation(scene, group_1, nullptr);
    //Station->setParentItem(scene);
    scene->addItem(Station);
    Station->setPos(sceneW/2, sceneH/2);

    auv = new AUV(scene, group_2, nullptr);
    auv->setParentItem(Station);
    scene->addItem(auv);
    auv->setPos(0, 0);

    vAngle = new viewingAngle(scene, group_2, nullptr, 50);
    vAngle->setParentItem(auv);
    scene->addItem(vAngle);
    vAngle->setPos(auv->auvLen/2, 0);


    //bool bOk = QObject::connect(auv, SIGNAL(sendCoords()), this, SLOT(receiveCoords()));
    bool bOk = QObject::connect(auv, &AUV::sendCoords, this, &MainWindow::receiveCoords);
    bOk = bOk && QObject::connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::receiveViewingAngle);
    Q_ASSERT(bOk);
}


MainWindow::~MainWindow()
{
    delete ui;
}

