#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMessageBox>
//#include <QKeyEvent>
#include <QDebug>
#include <QWidget>
#include <QGraphicsView>
#include <QTimer>

#include <QSlider>

#include <scene.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    MyScene      *myScene;
private:
    Ui::MainWindow *ui;

    /* Таймер для задержки отрисовки.
     * Дело в том, что при создании окна и виджета
     * необходимо некоторое время, чтобы родительский слой
     * развернулся, чтобы принимать от него адекватные параметры
     * ширины и высоты
     * */
    QTimer              *timer;

    int sceneW, sceneH;

protected:
   //virtual void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    void receiveCoords(int x, int y, int yaw);
    void receiveViewingAngle(int angle);
    void setButtonStartAnimationActive();
    void showInformationMessage(QString);

};
#endif // MAINWINDOW_H
