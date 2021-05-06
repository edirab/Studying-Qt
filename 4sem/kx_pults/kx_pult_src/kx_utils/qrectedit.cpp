#include "qrectedit.h"


QRectEdit::QRectEdit(QWidget * parent): QWidget(parent), lay(QBoxLayout::LeftToRight, this) {
	s_x = new QDoubleSpinBox(this);
	s_y = new QDoubleSpinBox(this);
	s_w = new QDoubleSpinBox(this);
	s_h = new QDoubleSpinBox(this);
	s_x->setMinimum(-DBL_MAX);
	s_x->setMaximum(DBL_MAX);
	s_y->setMinimum(-DBL_MAX);
	s_y->setMaximum(DBL_MAX);
	s_w->setMinimum(-DBL_MAX);
	s_w->setMaximum(DBL_MAX);
	s_h->setMinimum(-DBL_MAX);
	s_h->setMaximum(DBL_MAX);
	s_x->setToolTip(tr("X"));
	s_y->setToolTip(tr("Y"));
	s_w->setToolTip(tr("Height"));
	s_h->setToolTip(tr("Width"));
	lbl_0 = new QLabel(this);
	lbl_0->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	lbl_0->setText("x");
	lbl_1 = new QLabel(this);
	lbl_1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	lbl_1->setText(";");
	lbl_2 = new QLabel(this);
	lbl_2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	lbl_2->setText("x");
	lay.setContentsMargins(0, 0, 0, 0);
	lay.setSpacing(1);
	lay.addWidget(s_x);
	lay.addWidget(lbl_0);
	lay.addWidget(s_y);
	lay.addWidget(lbl_1);
	lay.addWidget(s_w);
	lay.addWidget(lbl_2);
	lay.addWidget(s_h);
	connect(s_x, SIGNAL(valueChanged(double)), this, SLOT(changed()));
	connect(s_y, SIGNAL(valueChanged(double)), this, SLOT(changed()));
	connect(s_w, SIGNAL(valueChanged(double)), this, SLOT(changed()));
	connect(s_h, SIGNAL(valueChanged(double)), this, SLOT(changed()));
}


void QRectEdit::changeEvent(QEvent * e) {
	if (e->type() == QEvent::LanguageChange) {
		s_x->setToolTip(tr("X"));
		s_y->setToolTip(tr("Y"));
		s_w->setToolTip(tr("Height"));
		s_h->setToolTip(tr("Width"));
		return;
	}
	QWidget::changeEvent(e);
}
