#include "pathdialog.h"
#include "ui_pathdialog.h"

PathDialog::PathDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PathDialog)
{
    path = new QString("");
    ui->setupUi(this);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(okPressed()));
    //connect(ui->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(okPressed()));
}

PathDialog::~PathDialog()
{
    delete ui;
}

void PathDialog::okPressed()
{
    //emit okSignal(ui->edtPath->text());
    //close();
}
