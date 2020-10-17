#ifndef PATHDIALOG_H
#define PATHDIALOG_H

#include <QDialog>

namespace Ui {
class PathDialog;
}

class PathDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PathDialog(QWidget *parent = 0);
    ~PathDialog();

private:
    Ui::PathDialog *ui;
    QString *path;

signals:
    void okSignal();
    void cancelPressed();
public slots:
    void okPressed();
};

#endif // PATHDIALOG_H
