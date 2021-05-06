#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

QScopedPointer <QFile> p_logFile;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    p_logFile.reset( new QFile("logfile.txt"));
    p_logFile.data()->open(QFile::Append | QFile::Text);
    qInstallMessageHandler(messageHandler);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg){

    QTextStream out(p_logFile.data());
    switch (type) {

    case QtDebugMsg: out << "Debug: "; break;
    case QtInfoMsg: out << "Info: "; break;
    case QtWarningMsg: out << "Warning: "; break;
    case QtCriticalMsg: out << "Critical error: "; break;
    case QtFatalMsg: out << "Fatal error: "; break;
    default: out << "Unknown: "; break;

    }
    out << context.category << " : " << msg << endl;
    out.flush();
}
