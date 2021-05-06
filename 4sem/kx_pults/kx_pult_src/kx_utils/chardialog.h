#ifndef CHARDIALOG_H
#define CHARDIALOG_H

#include <QDialog>
#include <QMetaEnum>
#include <QDebug>

namespace Ui {
	class CharDialog;
};

class CharDialog: public QDialog
{
	Q_OBJECT
public:
	explicit CharDialog(QWidget * parent = 0);
	~CharDialog();
	
	QChar selectedChar() {return sel_char;}
	void setCharFont(const QFont & f);
	
public slots:
	
private:
	void changeEvent(QEvent * e);
	virtual bool eventFilter(QObject * o, QEvent * e);
	virtual void resizeEvent(QResizeEvent * );
	void clear();
	
	Ui::CharDialog * ui;
	QVector<QVector<QChar> > chars;
	QVector<QChar> * cur;
	QChar sel_char;
	int size, csize;
	
private slots:
	void on_comboCategory_currentIndexChanged(int index);
	void on_verticalScroll_valueChanged(int index);
	void on_spinSize_valueChanged(int index);
	void on_tableChars_cellPressed(int row, int column);
	void on_tableChars_cellDoubleClicked(int , int ) {on_buttonBox_accepted();}
	void on_buttonBox_accepted();
	void on_buttonBox_rejected() {reject();}

signals:
	void charSelected(QChar ch);
	
};

#endif // CHARDIALOG_H
