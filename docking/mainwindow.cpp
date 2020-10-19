#include "mainwindow.h"
#include "ui_mainwindow.h"



void MainWindow::keyPressEvent(QKeyEvent *event) {

    auv->keyboardRedraw(event);
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
    this->scene->setSceneRect(0,0,this->ui->graphicsView->width(), this->ui->graphicsView->height());
    Station = new chargingStation(scene, group_1, nullptr);
    auv = new AUV(scene, group_2, nullptr);

    Station->draw();
    auv->paint();
    //chargingStation Station(scene, group_1, nullptr);

}


MainWindow::~MainWindow()
{
    delete ui;
}

