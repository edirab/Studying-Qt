#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMovie>

namespace LABEL {

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QLabel *lblText;
    QLabel *lblImgTxt;
    QLabel *lblImg;
    QLabel *lblImgSized;
    QLabel *lblImgSizedTxt;
    QLabel *lblMovTxt;
    QLabel *lblMov;

    QMovie *mov;
};

}
#endif // WIDGET_H
