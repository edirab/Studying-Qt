#include "dialog.h"
#include <QPushButton>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent) {
    path = new QString("");
    setupUi(this);
    connect (btnBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
             this, SLOT(ok_clicked()));
    connect (btnBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
             this, SLOT(close()));

}

Dialog::~Dialog() {

}

void Dialog::ok_clicked(){
   //path=edtPath->
   // if (path)
    emit ok(edtPath->text());
    close();
}

