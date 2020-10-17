#ifndef VMA_H
#define VMA_H

#include <QFrame>
#include "ui_vmaframechallenge.h"

class vma : public QFrame, public Ui::Frame
{
    Q_OBJECT

public:
    vma(QWidget *parent = nullptr);
    ~vma();

private:
    int marchValue;

public slots:
    void slVMAvalueChanged(int value);

};
#endif // VMA_H
