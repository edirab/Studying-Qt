#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    connect(btnLaunch, SIGNAL(clicked()),SIGNAL(missionLaunched()));
    connect(btnStop, SIGNAL(clicked()),SIGNAL(missionStopped()));
    connect(btnContinue,SIGNAL(clicked()), SIGNAL(missionContinue()));
    connect(btnDone,SIGNAL(clicked()),SIGNAL(missionDone()));
    connect(btnError,SIGNAL(clicked(bool)),SIGNAL(missionError()));
    connect(btnErrorSolved,SIGNAL(clicked(bool)),SIGNAL(missionErrorSolved()));
    connect(btnCable,SIGNAL(clicked(bool)),SIGNAL(cableFound()));
    connect(btnTrajectory,SIGNAL(clicked(bool)),SIGNAL(trajectoryDone()));

    idle = new QState();
    execute = new QState();
    stop = new QState();

    executeManeuver = new QState(execute);
    executeKAS = new QState(execute);

    hs = new QHistoryState(execute);
    hs->setDefaultState(executeManeuver);

    missionDoneState = new QFinalState(execute);
    execute->setInitialState(executeManeuver);

    idle->assignProperty(this, "m_state", "idle");
    execute->assignProperty(this, "m_state", "execute");
    stop->assignProperty(this, "m_state", "stop");

    executeManeuver->assignProperty(this, "m_state", "executeManeuver");
    executeKAS->assignProperty(this, "m_state", "executeKAS");

    idle->addTransition(this, SIGNAL(missionLaunched()), execute);
    execute->addTransition(this, SIGNAL(missionStopped()), stop);
    // execute->addTransition(this, SIGNAL(missionDone()), idle);
    stop->addTransition(this, SIGNAL(missionContinue()), hs);
    executeManeuver->addTransition(this, SIGNAL(missionDone()),missionDoneState);
    executeManeuver->addTransition(this, SIGNAL(missionError()),executeKAS);
    executeKAS->addTransition(this, SIGNAL(missionErrorSolved()),executeManeuver);
    execute->addTransition(this, SIGNAL(missionDone()),idle);

    stateMachine.addState(idle);
    stateMachine.addState(execute);
    stateMachine.addState(stop);

    stateMachine.setInitialState(idle);
    stateMachine.start();

}
