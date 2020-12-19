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
        QPoint hSliderCurrentPos = this->ui->hSlider_timeline->pos();
        QPoint labelTimelineCurrentPos = this->ui->label_timeline->pos();

        QSize grahicsViewCurrSize = this->ui->graphicsView->size();

        this->ui->graphicsView->resize(grahicsViewCurrSize.width()+deltaX, grahicsViewCurrSize.height()+deltaY);
        this->ui->groupBoxCamera->move(camGroupBoxCurrentPos.x() + deltaX, camGroupBoxCurrentPos.y());
        this->ui->groupBoxPosition->move(posGroupBoxCurrentPos.x() + deltaX, posGroupBoxCurrentPos.y());
        this->ui->labelHint->move(labelHintCurrentPos.x() + deltaX, labelHintCurrentPos.y());

        //this->ui->hSlider_timeline->setSizeIncrement(deltaX, 0);
        this->ui->hSlider_timeline->move(hSliderCurrentPos.x(), hSliderCurrentPos.y() + deltaY);
        this->ui->label_timeline->move(labelTimelineCurrentPos.x() + deltaX, labelTimelineCurrentPos.y() + deltaY);

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
    this->ui->hSlider_timeline->setEnabled(true);

    QString start_hint = "<html> \
                        <head/> \
                        <body> \
                        <p align=\"center\">Нажмите \"Начать обход\"</p> \
                        <p align=\"center\">для запуска отображения</p> \
                        </body></html>";

    this->ui->labelHint->setText(start_hint);
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
    this->ui->lineEdit_X->setText(QString::number(X_));
    this->ui->lineEdit_Y->setText(QString::number(Z_));
    this->ui->lineEdit_Yaw->setText(QString::number(Yaw));
}

/*
    Slot 6
*/
void MainWindow::updateSliderAndLabel(int currentStep, int total){
    //qDebug() << "Setting slider step... \n";
    this->ui->hSlider_timeline->setValue(currentStep);
    QString labelText = QString::number(currentStep) + "/" + QString::number(total);
    this->ui->label_timeline->setText(labelText);
}

/*
    Slot 7
*/
void MainWindow::customStartAmin(){
    qDebug() << "Обработчик кнопки запуска/останова анимации: " << state;

    QString stop_continue_hint = "<html> \
                        <head/> \
                        <body> \
                        <p align=\"center\">Перемещайте нижний ползунок</p> \
                        </body></html>";

    if (this->ui->labelHint->text() != stop_continue_hint){
            this->ui->labelHint->setText(stop_continue_hint);
    }

    if (state == 0){
        state = 1;
        emit startAnimation();
        this->ui->pushButton_startDocking->setText(QString("Пауза"));
    } else {
        state = 0;
        emit pauseAmination();
        this->ui->pushButton_startDocking->setText(QString("Продолжить"));
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
    this->ui->pushButton_startDocking->setEnabled(false);
    this->ui->hSlider_timeline->setEnabled(false);

    /*
    Соединяем сигналы со слотами
    */
    bool bOk = QObject::connect(myScene->auv, SIGNAL(sendCoords(int, int, int)), this, SLOT(receiveCoords(int, int, int)));
    //bool bOk = QObject::connect(myScene->auv, &AUV::sendCoords, this, &MainWindow::receiveCoords);
    bOk = bOk && QObject::connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::receiveViewingAngle);


    /*
    Загружаем файл из главного окна,
    проверяем корректность данных.
    Инициируем отрисовку
    */
    bOk = bOk && QObject::connect(ui->pushButton_loadFile, &QPushButton::clicked, myScene, &MyScene::readFile);
    bOk = bOk && QObject::connect(this->myScene , &MyScene::fileReadSuccessful, this, &MainWindow::setButtonStartAnimationActive);
    bOk = bOk && QObject::connect(this->myScene , &MyScene::fileReadFailed, this, &MainWindow::showInformationMessage);
    bOk = bOk && QObject::connect(ui->pushButton_startDocking, &QPushButton::toggled, this, &MainWindow::customStartAmin);


    /*
    Возможные ситуативные действия во время автоматически запущенной анимации
    */
    bOk = bOk && QObject::connect(this->myScene , &MyScene::sendCoordsDuringAnimation, this, &MainWindow::receiveCoordsDuringAnimation);
    bOk = bOk && QObject::connect(this->myScene , &MyScene::sendCurrentIterationStep, this, &MainWindow::updateSliderAndLabel);
    bOk = bOk && QObject::connect(ui->checkBox_showTrajectory, &QCheckBox::clicked, myScene , &MyScene::toggleTrajectory);

    /*
    Способы остановить анимацию
    */
    bOk = bOk && QObject::connect(ui->hSlider_timeline, &QSlider::sliderPressed, myScene, &MyScene::stopAnimTimer);
    bOk = bOk && QObject::connect(ui->hSlider_timeline, &QSlider::sliderPressed, [this]() {
        this->state = 0;
        this->ui->pushButton_startDocking->setChecked(false);
        this->ui->pushButton_startDocking->setText("Продолжить");
    });
    bOk = bOk && QObject::connect(this, &MainWindow::startAnimation, myScene, &MyScene::startAminTimer);
    bOk = bOk && QObject::connect(this, &MainWindow::pauseAmination, myScene, &MyScene::stopAnimTimer);

    bOk = bOk && QObject::connect(ui->hSlider_timeline, &QSlider::sliderMoved, myScene, &MyScene::sliderMoved);
    bOk = bOk && QObject::connect(ui->hSlider_timeline, &QSlider::sliderMoved, myScene, &MyScene::stopAnimTimer);

    Q_ASSERT(bOk);
}


MainWindow::~MainWindow()
{
    delete ui;
}

