#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <QDebug>


void drawDockingStation(QGraphicsItemGroup *group, QGraphicsScene *sc /*QGraphicsScene* sc*/){

    /* Приступаем к отрисовке произвольной картинки
     * */
    QPen penBlack(Qt::black); // Задаём чёрную кисть
    QPen penRed(Qt::red);   // Задаём красную кисть



    int width = sc->width();//  100;
    int height = sc->height();
    qDebug() << "Размеры GraphicsView: "<< width << " " << height;

    float centerx = width / 2;
    float centery = height / 2;

    int dockSize = 100;

    group->addToGroup(sc->addRect(centerx-dockSize/2,
                                  centery-dockSize/2,
                                  dockSize, dockSize, penBlack));

    group->addToGroup(sc->addRect(centerx-dockSize/4,
                                  centery-dockSize/4,
                                  dockSize/2, dockSize/2, penBlack));

    // 1
    group->addToGroup(sc->addLine(centerx-dockSize/2,
                                  centery-dockSize/2,
                                  centerx-dockSize/4,
                                  centery-dockSize/4, penBlack));
    // 2
    group->addToGroup(sc->addLine(centerx-dockSize/2 + dockSize,
                                  centery-dockSize/2,
                                  centerx-dockSize/4 + dockSize / 2,
                                  centery-dockSize/4, penBlack));

    // 3
    group->addToGroup(sc->addLine(centerx-dockSize/2,
                                  centery+dockSize/2,
                                  centerx-dockSize/4,
                                  centery+dockSize/4, penBlack));

    // 4
    group->addToGroup(sc->addLine(centerx+dockSize/2,
                                  centery+dockSize/2,
                                  centerx+dockSize/2 - dockSize/4,
                                  centery+dockSize/4, penBlack));

}

void test(){


}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene      *scene;     // Объявляем сцену для отрисовки
    QGraphicsItemGroup  *group_1;   // Объявляем первую группу элементов
    QGraphicsItemGroup  *group_2;
    /* Таймер для задержки отрисовки.
     * Дело в том, что при создании окна и виджета
     * необходимо некоторое время, чтобы родительский слой
     * развернулся, чтобы принимать от него адекватные параметры
     * ширины и высоты
     * */
    QTimer              *timer;

    scene = new QGraphicsScene();   // Инициализируем сцену для отрисовки
    this->ui->graphicsView->setScene(scene);

    group_1 = new QGraphicsItemGroup(); // Инициализируем первую группу элементов
    group_2 = new QGraphicsItemGroup(); // Инициализируем вторую группу элементов

    scene->addItem(group_1);            // Добавляем первую группу в сцену
    scene->addItem(group_2);            // Добавляем вторую группу в сцену

    timer = new QTimer();               // Инициализируем Таймер
    timer->setSingleShot(true);
    // Подключаем СЛОТ для отрисовки к таймеру
    //connect(timer, SIGNAL(timeout()), this, SLOT(slotAlarmTimer()));
    timer->start(50);                   // Стартуем таймер на 50 миллисекунд

    /* Устанавливаем размер сцены по размеру виджета
     * Первая координата - это левый верхний угол,
     * а Вторая - это правый нижний угол
     * */
    scene->setSceneRect(0,0,this->ui->graphicsView->width(), this->ui->graphicsView->height());

    drawDockingStation(group_1, scene);
//    /* Нарисуем красный квадрат
//     * */
//    int sideOfSquare =  (height > width) ? (width - 60) : (height - 60);
//    int centerOfWidget_X = width/2;
//    int centerOfWidget_Y = height/2;

//    group_2->addToGroup(scene->addLine(centerOfWidget_X - (sideOfSquare/2),
//                                       centerOfWidget_Y - (sideOfSquare/2),
//                                       centerOfWidget_X + (sideOfSquare/2),
//                                       centerOfWidget_Y - (sideOfSquare/2),
//                                       penRed));

//    group_2->addToGroup(scene->addLine(centerOfWidget_X + (sideOfSquare/2),
//                                       centerOfWidget_Y - (sideOfSquare/2),
//                                       centerOfWidget_X + (sideOfSquare/2),
//                                       centerOfWidget_Y + (sideOfSquare/2),
//                                       penRed));

//    group_2->addToGroup(scene->addLine(centerOfWidget_X + (sideOfSquare/2),
//                                       centerOfWidget_Y + (sideOfSquare/2),
//                                       centerOfWidget_X - (sideOfSquare/2),
//                                       centerOfWidget_Y + (sideOfSquare/2),
//                                       penRed));

//    group_2->addToGroup(scene->addLine(centerOfWidget_X - (sideOfSquare/2),
//                                       centerOfWidget_Y + (sideOfSquare/2),
//                                       centerOfWidget_X - (sideOfSquare/2),
//                                       centerOfWidget_Y - (sideOfSquare/2),
//                                       penRed));


}

MainWindow::~MainWindow()
{
    delete ui;
}

