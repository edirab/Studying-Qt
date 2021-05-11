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
        //QPoint hSliderCurrentPos = this->ui->hSlider_timeline->pos();
        //QPoint labelTimelineCurrentPos = this->ui->label_timeline->pos();

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
    Slot 4
*/
void MainWindow::showInformationMessage(QString error_msg){
    //qDebug() << "Showing info massage \n";

    QString msg = "Oh-la-la";
    QMessageBox::StandardButton reply;
    reply = QMessageBox::critical /*information*/(this, tr("Ошибка во входных данных"), error_msg);
}

/*
    Slot 5
*/
void MainWindow::receiveCoordsDuringAnimation(float X_, float Z_, float Yaw){
    this->ui->lineEdit_X->setText(QString::number(X_, 'f', 1));
    this->ui->lineEdit_Y->setText(QString::number(Z_, 'f', 1));
    this->ui->lineEdit_Yaw->setText(QString::number(Yaw, 'f', 2));
}


/*
    Slot 7
*/
void MainWindow::customStartAmin(){
    qDebug() << "Обработчик кнопки запуска/останова анимации: " << state;

    double V_fwd = (this->ui->lineEdit_Vfwd->text().toDouble());
    double k1_m = (this->ui->lineEdit_k1_m->text().toDouble());
    double k2_m = (this->ui->lineEdit_k2_m->text().toDouble());
    double k1_yaw = (this->ui->lineEdit_k1_yaw->text().toDouble());
    double k2_yaw = (this->ui->lineEdit_k2_yaw->text().toDouble());

    emit send_su_params(V_fwd, k1_m, k2_m, k1_yaw, k2_yaw);

    qDebug() <<"In Main Waindow: " << V_fwd << " " << k1_m << " " << k2_m << " " << k1_yaw << " " << k2_yaw << "\n";


    QString stop_continue_hint = "<html> \
                        <head/> \
                        <body> \
                        <p align=\"center\"></p> \
                        </body></html>";

    if (this->ui->labelHint->text() != stop_continue_hint){
            this->ui->labelHint->setText(stop_continue_hint);
    }

    if (state == 0){
        state = 1;
        emit startAnimation();
        this->ui->pushButton_startDocking->setText(QString("Пауза"));
        this->ui->lineEdit_Vfwd->setEnabled(false);
        this->ui->lineEdit_k1_m->setEnabled(false);
        this->ui->lineEdit_k2_m->setEnabled(false);
        this->ui->lineEdit_k1_yaw->setEnabled(false);
        this->ui->lineEdit_k2_yaw->setEnabled(false);

    } else {
        state = 0;
        emit pauseAmination();
        this->ui->pushButton_startDocking->setText(QString("Продолжить"));
        this->ui->lineEdit_Vfwd->setEnabled(true);
        this->ui->lineEdit_k1_m->setEnabled(true);
        this->ui->lineEdit_k2_m->setEnabled(true);
        this->ui->lineEdit_k1_yaw->setEnabled(true);
        this->ui->lineEdit_k2_yaw->setEnabled(true);
    }
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

    /*
        Соединяем сигналы со слотами
    */
    bool bOk = QObject::connect(myScene->auv, &AUV::sendCoords, this, &MainWindow::receiveCoords);
    bOk = bOk && QObject::connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::receiveViewingAngle);
    bOk = bOk && QObject::connect(this, &MainWindow::send_su_params, this->myScene->auv, &AUV::receive_su_params);

    /*
        Инициируем отрисовку
    */
    bOk = bOk && QObject::connect(ui->pushButton_startDocking, &QPushButton::clicked, this, &MainWindow::customStartAmin);


    /*
    Возможные ситуативные действия во время автоматически запущенной анимации
    */
    bOk = bOk && QObject::connect(this->myScene , &MyScene::sendCoordsDuringAnimation, this, &MainWindow::receiveCoordsDuringAnimation);

    /*
        Способы остановить анимацию
    */

    bOk = bOk && QObject::connect(this, &MainWindow::startAnimation, myScene, &MyScene::startAminTimer);
    bOk = bOk && QObject::connect(this, &MainWindow::pauseAmination, myScene, &MyScene::stopAnimTimer);

    Q_ASSERT(bOk);
}


MainWindow::~MainWindow()
{
    delete ui;
}

