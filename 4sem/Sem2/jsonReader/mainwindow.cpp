#include "mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <qlogging.h>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    QJsonDocument jsonDoc;

    QFile logFile("logfile.txt");

    QFile configFile("config.json");
    if (configFile.exists() && logFile.exists()){

        configFile.open((QFile::ReadOnly));
        logFile.open(QFile::ReadOnly);

        qDebug() << "config.json opened";

        jsonDoc = QJsonDocument().fromJson(configFile.readAll());
        txtConfig->setText(jsonDoc.toJson());

        QJsonObject obj = jsonDoc.object();
        qInfo() << "ip:port = " << obj.value("ip").toVariant().toString() <<
                   ":" << obj.value("port").toVariant().toString();
        qInfo() << "frequency= " << obj.value("frequency").toVariant().toString();

        txtLog->setText(logFile.readAll());
        configFile.close();
        logFile.close();
    } else {
        if (!logFile.exists()) qFatal("No file: logFile.txt");
        if (!configFile.exists()) qFatal("No file: config.json");
    }
}
