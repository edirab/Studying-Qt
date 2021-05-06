#include "qpointedit.h"


QPointEdit::QPointEdit(QWidget * parent): QWidget(parent), lay(QBoxLayout::LeftToRight, this) {
	s_x = new QDoubleSpinBox(this);
	s_y = new QDoubleSpinBox(this);
	s_x->setMinimum(-DBL_MAX);
	s_x->setMaximum(DBL_MAX);
	s_y->setMinimum(-DBL_MAX);
	s_y->setMaximum(DBL_MAX);
	s_x->setToolTip(tr("X"));
	s_y->setToolTip(tr("Y"));
	lbl = new QLabel(this);
	lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	lbl->setText("x");
	lay.setContentsMargins(0, 0, 0, 0);
	lay.setSpacing(1);
	lay.addWidget(s_x);
	lay.addWidget(lbl);
	lay.addWidget(s_y);
	connect(s_x, SIGNAL(valueChanged(double)), this, SLOT(changed()));
	connect(s_y, SIGNAL(valueChanged(double)), this, SLOT(changed()));
}


void QPointEdit::changeEvent(QEvent * e) {
	if (e->type() == QEvent::LanguageChange) {
		s_x->setToolTip(tr("X"));
		s_y->setToolTip(tr("Y"));
		return;
	}
	QWidget::changeEvent(e);
}
