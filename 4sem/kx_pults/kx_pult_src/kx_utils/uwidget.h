#ifndef UWIDGET_H
#define UWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QEvent>


class UWidget: public QWidget
{
	Q_OBJECT
public:
	UWidget(QWidget * parent = 0): QWidget(parent) {};
	
private:
	virtual bool event(QEvent * e) {
		if (e->type() != QEvent::Paint) return QWidget::event(e);
		e->accept();
		QStyleOption opt;
		opt.init(this);
		QPainter p(this);
		style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
		emit paintEvent((QPaintEvent * )e);
		return true;
	}

signals:
	void closeEvent(QCloseEvent * e);
	void dragEnterEvent(QDragEnterEvent * e);
	void dragLeaveEvent(QDragLeaveEvent * e);
	void dragMoveEvent(QDragMoveEvent * e);
	void dropEvent(QDropEvent * e);
	void enterEvent(QEvent * e);
	void hideEvent(QHideEvent * e);
	void keyPressEvent(QKeyEvent * e);
	void keyReleaseEvent(QKeyEvent * e);
	void leaveEvent(QEvent * e);
	void mouseDoubleClickEvent(QMouseEvent * e);
	void mouseMoveEvent(QMouseEvent * e);
	void mousePressEvent(QMouseEvent * e);
	void mouseReleaseEvent(QMouseEvent * e);
	void moveEvent(QMoveEvent * e);
	void resizeEvent(QResizeEvent * e);
	void showEvent(QShowEvent * e);
	void wheelEvent(QWheelEvent * e);
	void paintEvent(QPaintEvent * e);

};

#endif
