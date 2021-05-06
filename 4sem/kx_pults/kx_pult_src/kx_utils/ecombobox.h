#ifndef ECOMBOBOX_H
#define ECOMBOBOX_H

#include <QComboBox>
#include <QBoxLayout>
#include <QTreeView>
#include <QLabel>
#include "clineedit.h"


class EComboBox: public QComboBox
{
	Q_OBJECT
public:
	explicit EComboBox(QWidget * parent = 0);
	
	QSize sizeHint() const;

public slots:
	virtual void showPopup();
	
private:
	QTreeView iv;
	QWidget header;
	QLabel icon;
	CLineEdit filter;
	QFont nfont, ifont;
	
private slots:
	void filterChanged(const QString & text, bool first = false);
	
signals:
	
};

#endif // ECOMBOBOX_H
