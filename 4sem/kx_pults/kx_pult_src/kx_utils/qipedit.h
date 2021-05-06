#ifndef QIPEDIT_H
#define QIPEDIT_H

#include <QVector>
#include <QBoxLayout>
#include <QIntValidator>
#include <QFocusEvent>
#include <QLineEdit>
#include <QLabel>

class QIPEdit: public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QString IP READ IP WRITE setIP)
	
public:
	QIPEdit(QWidget * parent = 0, const QString & ip = "");
	~QIPEdit();
	
	QString IP();

private:
	void returnPress(int index);
	inline void textChange(int index, const QString & text) {if (text.length() == 3 && isVisible()) returnPress(index); emit valueChanged(IP());}

	int cind;
	QBoxLayout * layout;
	QVector<QLineEdit * > edits;
	QVector<QLabel * > dots;
	
public slots:
	void setIP(const QString & text);
	
private slots:
	void returnPressed0() {returnPress(0);}
	void returnPressed1() {returnPress(1);}
	void returnPressed2() {returnPress(2);}
	void returnPressed3() {returnPress(3);}
	void textChanged0(const QString & text) {textChange(0, text);}
	void textChanged1(const QString & text) {textChange(1, text);}
	void textChanged2(const QString & text) {textChange(2, text);}
	void textChanged3(const QString & text) {textChange(3, text);}
	
signals:
	void valueChanged(QString);
};

#endif // QIPEDIT_H
