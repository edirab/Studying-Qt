#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QHistoryState>
#include <QFinalState>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
    Q_PROPERTY(QString m_state READ state WRITE setState)

signals:
    void missionLaunched();
    void missionStopped();
    void missionContinue();
    void missionDone();
    void missionError();
    void missionErrorSolved();
    void cableFound();
    void trajectoryDone();

public:
    explicit MainWindow(QWidget *parent = 0);
    //метод, для чтения значения переменной состояния системы
    QString state(){return m_state;}
    //метод для записи нового значения перменной состояния системы
    void setState(QString st){
        m_state = st;
        textBrowser->append(st);
    }
private:
    QString m_state;
    QState *idle, *execute, *stop;
    QStateMachine stateMachine;
    QState *executeManeuver, *executeKAS;
    QHistoryState *hs;
    QFinalState *missionDoneState;


};

#endif // MAINWINDOW_H
