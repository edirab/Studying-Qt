#ifndef QPICONFIGNEWDIALOG_H
#define QPICONFIGNEWDIALOG_H

#include <QDialog>
#include <QRadioButton>


namespace Ui {
	class QPIConfigNewDialog;
}


class QPIConfigNewDialog: public QDialog
{
	Q_OBJECT
	
public:
	QPIConfigNewDialog(QWidget * parent = 0);
	
	QString type();
	QString name();
	QString value();
	QString comment();
	void reset(bool node = false);
	
protected:
	void changeEvent(QEvent * e);
	
	Ui::QPIConfigNewDialog * ui;
	
private slots:
	void on_lineName_textChanged(const QString & text);
	void typeChanged();

private:
	QList<QRadioButton * > radios;
	
};

#endif // QPICONFIGNEWDIALOG_H
