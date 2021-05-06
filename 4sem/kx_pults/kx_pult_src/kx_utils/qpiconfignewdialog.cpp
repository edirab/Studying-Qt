#include "qpiconfignewdialog.h"
#include "ui_qpiconfignewdialog.h"


QPIConfigNewDialog::QPIConfigNewDialog(QWidget * parent): QDialog(parent) {
	ui = new Ui::QPIConfigNewDialog();
	ui->setupUi(this);
	radios = findChildren<QRadioButton * >();
	ui->widgetValue->hideAll();
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}


void QPIConfigNewDialog::changeEvent(QEvent * e) {
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}


void QPIConfigNewDialog::typeChanged() {
	foreach (QRadioButton * i, radios) {
		if (i->isChecked()) {
			ui->widgetValue->setType(i->property("type").toString());
			return;
		}
	}
}


QString QPIConfigNewDialog::type() {
	foreach (QRadioButton * i, radios)
		if (i->isChecked())
			return i->property("type").toString();
	return " ";
}


void QPIConfigNewDialog::reset(bool node) {
	ui->lineName->clear();
	ui->lineComment->clear();
	ui->radioType_0->setChecked(true);
	ui->lineName->setFocus();
	ui->widgetValue->setType("s");
	ui->widgetValue->value.clear();
	ui->widgetValue->setVisible(!node);
	ui->groupType->setVisible(!node);
	ui->labelValue->setVisible(!node);
	ui->labelComment->setVisible(!node);
	ui->lineComment->setVisible(!node);
	adjustSize();
}


QString QPIConfigNewDialog::name() {
	return ui->lineName->text();
}


QString QPIConfigNewDialog::value() {
	return ui->widgetValue->value;
}


QString QPIConfigNewDialog::comment() {
	return ui->lineComment->text();
}


void QPIConfigNewDialog::on_lineName_textChanged(const QString & text) {
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty());
}
