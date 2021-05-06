#ifndef PATHDIALOG_H
#define PATHDIALOG_H

#include <QDialog>
#include "ui_pathdialog.h"
namespace Ui {
class PathDialog;
}

class PathDialog : public QDialog, public Ui::PathDialog
{
    Q_OBJECT

public:
    explicit PathDialog(QWidget *parent = 0);
    ~PathDialog();

private:
    Ui::PathDialog *ui;
};

#endif // PATHDIALOG_H
