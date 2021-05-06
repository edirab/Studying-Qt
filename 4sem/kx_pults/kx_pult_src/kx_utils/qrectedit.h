#ifndef QRECTEDIT_H
#define QRECTEDIT_H

#include <QDoubleSpinBox>
#include <QBoxLayout>
#include <QLabel>
#include <QEvent>
#include "float.h"


class QRectEdit: public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QRectF value READ value WRITE setValue)
	Q_PROPERTY(int decimals READ decimals WRITE setDecimals)
	Q_PROPERTY(double maximum READ maximum WRITE setMaximum)
	Q_PROPERTY(double minimum READ minimum WRITE setMinimum)
	Q_PROPERTY(double singleStep READ singleStep WRITE setSingleStep)

public:
	explicit QRectEdit(QWidget * parent = 0);
	~QRectEdit() {delete s_x; delete s_y; delete s_w; delete s_h; delete lbl_0; delete lbl_1; delete lbl_2;}
	
	QRectF value() const {return QRectF(s_x->value(), s_y->value(), s_w->value(), s_h->value());}
	int decimals() const {return s_x->decimals();}
	double maximum() const {return s_x->maximum();}
	double minimum() const {return s_x->minimum();}
	double singleStep() const {return s_x->singleStep();}

public slots:
	void setValue(QRectF v) {s_x->setValue(v.x()); s_y->setValue(v.y()); s_w->setValue(v.width()); s_h->setValue(v.height());}
	void setDecimals(int d) {s_x->setDecimals(d); s_y->setDecimals(d); s_w->setDecimals(d); s_h->setDecimals(d);}
	void setMaximum(double m) {s_x->setMaximum(m); s_y->setMaximum(m); s_w->setMaximum(m); s_h->setMaximum(m);}
	void setMinimum(double m) {s_x->setMinimum(m); s_y->setMinimum(m); s_w->setMinimum(m); s_h->setMinimum(m);}
	void setSingleStep(double m) {s_x->setSingleStep(m); s_y->setSingleStep(m); s_w->setSingleStep(m); s_h->setSingleStep(m);}

private:
	virtual void changeEvent(QEvent * e);
	
	QBoxLayout lay;
	QDoubleSpinBox * s_x, * s_y, * s_w, * s_h;
	QLabel * lbl_0, * lbl_1, * lbl_2;
	
private slots:
	void changed() {emit valueChanged(QRectF(s_x->value(), s_y->value(), s_w->value(), s_h->value()));}
	
signals:
	void valueChanged(QRectF);
	
};

#endif // QRECTEDIT_H
