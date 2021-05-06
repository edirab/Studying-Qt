#include "colorbutton.h"
#include <QDebug>
#include <QDrag>
#include <QMimeData>

ColorButton::ColorButton(QWidget * parent): QPushButton(parent) {
	frame = false;
	options = QColorDialog::ShowAlphaChannel;
	back = new QWidget(this);
	back->setAutoFillBackground(true);
	back->show();
	pal = back->palette();
	pal.setBrush(back->backgroundRole(), QBrush(QImage(":/icons/alpha.png")));
	back->setPalette(pal);
	label = new QFrame(this);
	label->setAutoFillBackground(true);
	label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	label->show();
	pal = label->palette();
	menu.addAction(QIcon(":/icons/edit-copy.png"), tr("Copy"), this, SLOT(copy()));
	menu.addAction(QIcon(":/icons/edit-paste.png"), tr("Paste"), this, SLOT(paste()));
	menu.addSeparator();
	menu.addAction(tr("Mix with clipboard"), this, SLOT(mix()));
	setAcceptDrops(true);
	connect(this, SIGNAL(clicked(bool)), this, SLOT(clicked()));
}


ColorButton::~ColorButton() {
	delete label;
}


void ColorButton::resizeEvent(QResizeEvent * ) {
	if (frame) back->setGeometry(rect());
	else back->setGeometry(8, 5, width() - 16, height() - 12);
	label->setGeometry(back->geometry());
}


void ColorButton::mousePressEvent(QMouseEvent * e) {
	pp = e->pos();
	if (e->buttons().testFlag(Qt::RightButton)) {
		menu.popup(e->globalPos());
		return;
	}
	QPushButton::mousePressEvent(e);
}


void ColorButton::mouseMoveEvent(QMouseEvent * e) {
	if (e->buttons().testFlag(Qt::LeftButton)) {
		if ((e->pos() - pp).manhattanLength() > QApplication::startDragDistance()) {
			setDown(false);
			QDrag * drag = new QDrag(this);
			QMimeData * data = new QMimeData();
			data->setColorData(color());
			drag->setMimeData(data);
			drag->exec(Qt::CopyAction);
			return;
		}
	}
	QPushButton::mouseMoveEvent(e);
}


void ColorButton::dragEnterEvent(QDragEnterEvent * e) {
	e->accept();
	QPushButton::dragEnterEvent(e);
}


void ColorButton::dropEvent(QDropEvent * e) {
	const QMimeData * data = e->mimeData();
	QColor c = qvariant_cast<QColor>(data->colorData());
	if (c.isValid()) {
		setColor(c);
		return;
	}
	c = QColor(data->text());
	if (c.isValid()) {
		setColor(c);
		return;
	}
	QPushButton::dropEvent(e);
}


void ColorButton::clicked() {
	QColor ret = QColorDialog::getColor(color(), this, tr("Choose color"), options);
	if (!ret.isValid()) return;
	setColor(ret);
}


void ColorButton::mix() {
	QColor c(QApplication::clipboard()->text());
	if (!c.isValid()) return;
	QColor sc = color();
	setColor(QColor((c.red() + sc.red()) / 2, (c.green() + sc.green()) / 2, (c.blue() + sc.blue()) / 2, (c.alpha() + sc.alpha()) / 2));
}


void ColorButton::setColor(const QColor & col) {
	if (pal.color(label->backgroundRole()) == col) return;
	if (options.testFlag(QColorDialog::ShowAlphaChannel))
		pal.setColor(label->backgroundRole(), col);
	else
		pal.setColor(label->backgroundRole(), QColor(col.red(), col.green(), col.blue()));
	label->setPalette(pal);
	emit colorChanged(color());
}
