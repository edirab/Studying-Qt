#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile logFile("log.txt");
    QFile configFile("config.json");
    if (configFile.exists()){
        configFile.open(QFile::ReadOnly);
        qDebug() << "config.json opened";
    }
    logFile.open(QFile::ReadOnly);
    qDebug() << "log.txt opened";

    QJsonDocument jsonDoc = QJsonDocument().fromJson(configFile.readAll());
    ui->textConfig->setText(jsonDoc.toJson());

    QJsonObject jsonObject = jsonDoc.object();
    qInfo() << "ip:port = " << jsonObject.value("ip").toVariant().toString() <<
               ":" << jsonObject.value("frequency").toVariant().toString();
    ui->textLog->setText(logFile.readAll());
    configFile.close();
    logFile.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
