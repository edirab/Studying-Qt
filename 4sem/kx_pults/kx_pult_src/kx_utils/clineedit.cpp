#include "clineedit.h"
#include <QString>

CLineEdit::CLineEdit(QWidget * parent): QLineEdit(parent) {
	cw = new QWidget(this);
	cw->setStyleSheet("background-image: url(:/icons/edit-clear-locationbar-rtl.png);");
	cw->setCursor(Qt::ArrowCursor);
	cw->setToolTip(tr("Clear"));
	cw->hide();
	cw->installEventFilter(this);
	int m0, m1, m2, m3;
	getTextMargins(&m0, &m1, &m2, &m3);
	setTextMargins(m0, m1, m2 + 21, m3);
	connect(this, SIGNAL(textChanged(QString)), this, SLOT(textChanged_(QString)));
	//connect(cw, SIGNAL(mouseReleaseEvent(QMouseEvent * )), this, SLOT(clearMouseRelease(QMouseEvent * )));
}


bool CLineEdit::eventFilter(QObject * o, QEvent * e) {
	if (e->type() == QEvent::MouseButtonRelease) {
		clearMouseRelease((QMouseEvent * )e);
	}
	return QLineEdit::eventFilter(o, e);
}


void CLineEdit::resizeEvent(QResizeEvent * e) {
	QLineEdit::resizeEvent(e);
	cw->setGeometry(width() - 21, (height() - 17) / 2, 16, 16);
}


void CLineEdit::changeEvent(QEvent * e) {
	if (e->type() == QEvent::LanguageChange) {
		cw->setToolTip(tr("Clear"));
		return;
	}
	QLineEdit::changeEvent(e);
}


void CLineEdit::setDefaultText(const QString & t, bool set_text) {
	dt = t;
	if (set_text) {
        setText(t);
		emit textEdited(t);
		cw->hide();
		return;
	}
	textChanged_(text());
}
