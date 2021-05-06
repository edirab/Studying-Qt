#ifndef QPOINTEDIT_H
#define QPOINTEDIT_H

#include <QDoubleSpinBox>
#include <QBoxLayout>
#include <QLabel>
#include <QEvent>
#include "float.h"


class QPointEdit: public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QPointF value READ value WRITE setValue)
	Q_PROPERTY(int decimals READ decimals WRITE setDecimals)
	
public:
	explicit QPointEdit(QWidget * parent = 0);
	~QPointEdit() {delete s_x; delete s_y; delete lbl;}
	
	QPointF value() const {return QPointF(s_x->value(), s_y->value());}
	int decimals() const {return s_x->decimals();}
	
public slots:
	void setValue(QPointF v) {s_x->setValue(v.x()); s_y->setValue(v.y());}
	void setDecimals(int d) {s_x->setDecimals(d); s_y->setDecimals(d);}
	
private:
	virtual void changeEvent(QEvent * e);
	
	QBoxLayout lay;
	QDoubleSpinBox * s_x, * s_y;
	QLabel * lbl;
	
private slots:
	void changed() {emit valueChanged(QPointF(s_x->value(), s_y->value()));}
	
signals:
	void valueChanged(QPointF);
	
};

#endif // QPOINTEDIT_H
