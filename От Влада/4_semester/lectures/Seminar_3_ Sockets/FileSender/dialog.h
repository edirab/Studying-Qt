#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "ui_dialog.h"

class Dialog : public QDialog, public Ui::Dialog {

    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    QString *path;

signals:
    void ok(const QString &str);
    void cancel();
public slots:
    void ok_clicked();
};

#endif // DIALOG_H
