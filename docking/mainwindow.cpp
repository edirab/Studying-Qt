#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::resizeEvent(QResizeEvent *event){

    bool debugResize = false;
    if (event->oldSize() != QSize(-1, -1)){

        int deltaX = this->size().width() - event->oldSize().width();
        int deltaY = this->size().height() - event->oldSize().height();

        QPoint camGroupBoxCurrentPos = this->ui->groupBoxCamera->pos();
        QPoint posGroupBoxCurrentPos = this->ui->groupBoxPosition->pos();
        QPoint labelHintCurrentPos = this->ui->labelHint->pos();

        QSize grahicsViewCurrSize = this->ui->graphicsView->size();

        this->ui->graphicsView->resize(grahicsViewCurrSize.width()+deltaX, grahicsViewCurrSize.height()+deltaY);
        this->ui->groupBoxCamera->move(camGroupBoxCurrentPos.x() + deltaX, camGroupBoxCurrentPos.y());
        this->ui->groupBoxPosition->move(posGroupBoxCurrentPos.x() + deltaX, posGroupBoxCurrentPos.y());
        this->ui->labelHint->move(labelHintCurrentPos.x() + deltaX, labelHintCurrentPos.y());

        if (debugResize){
            qDebug() << "Main Window size: " << this->size() << ", Old size:  " << event->oldSize() << " delta: " << deltaX;
            qDebug() << camGroupBoxCurrentPos << " " << posGroupBoxCurrentPos;
            qDebug() << "New Pos: " << QPoint(camGroupBoxCurrentPos.x() + deltaX, camGroupBoxCurrentPos.y());
        }
 }
}


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

/*
    Slot 3
*/
void MainWindow::setButtonStartAnimationActive(){
    qDebug() << "Activate Start Anim Push Button \n";
    this->ui->pushButton_startDocking->setEnabled(true);

}


/*
    Slot 4
*/
void MainWindow::showInformationMessage(QString error_msg){
    qDebug() << "Showing info massage \n";

    QString msg = "Oh-la-la";
    QMessageBox::StandardButton reply;
    reply = QMessageBox::critical /*information*/(this, tr("Ошибка во входных данных"), error_msg);
}


// Constructor
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
    this->ui->pushButton_startDocking->setEnabled(false);

    /*
    Соединяем сигналы со слотами
    */
    bool bOk = QObject::connect(myScene->auv, SIGNAL(sendCoords(int, int, int)), this, SLOT(receiveCoords(int, int, int)));
    //bool bOk = QObject::connect(myScene->auv, &AUV::sendCoords, this, &MainWindow::receiveCoords);
    bOk = bOk && QObject::connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::receiveViewingAngle);

    bOk = bOk && QObject::connect(ui->pushButton_loadFile, &QPushButton::clicked, myScene, &MyScene::readFile);
    bOk = bOk && QObject::connect(ui->pushButton_startDocking, &QPushButton::clicked, myScene, &MyScene::startVisualization);
    bOk = bOk && QObject::connect(this->myScene , &MyScene::fileReadSuccessful, this, &MainWindow::setButtonStartAnimationActive);
    bOk = bOk && QObject::connect(this->myScene , &MyScene::fileReadFailed, this, &MainWindow::showInformationMessage);
    Q_ASSERT(bOk);
}


MainWindow::~MainWindow()
{
    delete ui;
}

