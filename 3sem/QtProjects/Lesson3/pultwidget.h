#ifndef PULTWIDGET_H
#define PULTWIDGET_H

#include <QFrame>
#include <QtUiTools>
#include <ui_form.h>
/*namespace Ui {
    class Form;
}*/

class PultWidget : public QFrame
{
    Q_OBJECT

public:
    PultWidget(QFrame *parent = nullptr);
    ~PultWidget();
private:
    //Ui::Form* ui;
};
#endif // PULTWIDGET_H
