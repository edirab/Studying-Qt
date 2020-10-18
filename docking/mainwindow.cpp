#include "mainwindow.h"
#include "ui_mainwindow.h"



void MainWindow::drawDockingStation(QGraphicsItemGroup *group, QGraphicsScene *sc /*QGraphicsScene* sc*/){

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


void MainWindow::keyPressEvent(QKeyEvent *event) {

 int key=event->key(); //event->key() - целочисленный код клавиши
 qDebug() << "Нажатие: " << key;

    // if (key==Qt::Key_Right) {
    //    auv_x += 10;
    // }
    // else if (key==Qt::Key_Left) {
    //    auv_x -= 10;
    // }
    // else if (key==Qt::Key_Up) {
    //    auv_y -= 10;
    // }
    // else if (key==Qt::Key_Down) {
    //    auv_y += 10;
    // }
//     if (key == Qt::Key_D) {
//        auv_x += 10;
//     }
//     else if (key == Qt::Key_A) {
//        auv_x -= 10;
//     }
//     else if (key == Qt::Key_W) {
//        auv_y -= 10;
//     }
//     else if (key == Qt::Key_S) {
//        auv_y += 10;
//     }

//   qDebug() << "auv_x: " << auv_x << "auv_y" << auv_y ;

   foreach( QGraphicsItem *item, scene->items(group_2->boundingRect())) {
      if(item->group() == group_2 ) {
         delete item;
      }
   }

   //this->drawAUV(this->group_2, this->scene);

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
    this->drawDockingStation(group_1, scene);
    AUV auv(scene, group_2, nullptr);
    auv.draw();
    //this->drawAUV(group_2, scene);

}

MainWindow::~MainWindow()
{
    delete ui;
}

