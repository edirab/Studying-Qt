#include "qipedit.h"

QIPEdit::QIPEdit(QWidget * parent, const QString & ip): QWidget(parent) {
	layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(1);
	QIntValidator * validator = new QIntValidator(0, 255, this);
	for (int i = 0; i < 4; i++) {
		edits.push_back(new QLineEdit(this));
		edits.back()->setAlignment(Qt::AlignHCenter);
		edits.back()->setMaxLength(3);
		edits.back()->setValidator(validator);
		edits.back()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		layout->addWidget(edits.back());
		if (i < 3) {
			dots.push_back(new QLabel(this));
			dots.back()->setText(".");
			dots.back()->adjustSize();
			layout->addWidget(dots.back());
		}
	}
	//for (int i = 0; i < 3; i++) edits[i]->setTabOrder(edits[i+1], edits[i]);
	connect(edits[0], SIGNAL(returnPressed()), this, SLOT(returnPressed0()));
	connect(edits[1], SIGNAL(returnPressed()), this, SLOT(returnPressed1()));
	connect(edits[2], SIGNAL(returnPressed()), this, SLOT(returnPressed2()));
	connect(edits[3], SIGNAL(returnPressed()), this, SLOT(returnPressed3()));
	connect(edits[0], SIGNAL(textChanged(const QString & )), this, SLOT(textChanged0(const QString & )));
	connect(edits[1], SIGNAL(textChanged(const QString & )), this, SLOT(textChanged1(const QString & )));
	connect(edits[2], SIGNAL(textChanged(const QString & )), this, SLOT(textChanged2(const QString & )));
	connect(edits[3], SIGNAL(textChanged(const QString & )), this, SLOT(textChanged3(const QString & )));
	setLayout(layout);
	setIP(ip);
	cind = 0;
}


QIPEdit::~QIPEdit() {
	foreach (QLineEdit * i, edits)
		delete i;
	foreach (QLabel * i, dots)
		delete i;
	edits.clear();
	dots.clear();
	delete layout;
}


void QIPEdit::setIP(const QString & text) {
	QString s, str = text;
	s = str.left(str.indexOf('.'));
	edits[0]->setText(s == "" ? "0" : s);
	str = str.right(str.length() - s.length() - 1);
	s = str.left(str.indexOf('.'));
	edits[1]->setText(s == "" ? "0" : s);
	str = str.right(str.length() - s.length() - 1);
	s = str.left(str.indexOf('.'));
	edits[2]->setText(s == "" ? "0" : s);
	str = str.right(str.length() - s.length() - 1);
	edits[3]->setText(str == "" ? "0" : str);
}


QString QIPEdit::IP() {
	QString s;
	if (edits[0]->text() == "") s = "0.";
	else s = edits[0]->text() + ".";
	if (edits[1]->text() == "") s += "0.";
	else s += edits[1]->text() + ".";
	if (edits[2]->text() == "") s += "0.";
	else s += edits[2]->text() + ".";
	if (edits[3]->text() == "") s += "0";
	else s += edits[3]->text();
	return s;
}


void QIPEdit::returnPress(int index) {
	if (index < 3) {
		edits[index + 1]->setFocus();
		edits[index + 1]->setSelection(0, 3);
	}
}
