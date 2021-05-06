#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>
#include <QFrame>
#include <QColorDialog>
#include <QMouseEvent>
#include <QAction>
#include <QMenu>
#include <QClipboard>
#include <QApplication>


class ColorButton: public QPushButton
{
	Q_OBJECT
	Q_PROPERTY(QColor color READ color WRITE setColor)
	Q_PROPERTY(bool useNativeDialog READ useNativeDialog WRITE setUseNativeDialog)
	Q_PROPERTY(bool useAlphaChannel READ useAlphaChannel WRITE setUseAlphaChannel)
	Q_PROPERTY(bool frameOnly READ frameOnly WRITE setFrameOnly)

public:
	explicit ColorButton(QWidget * parent = 0);
	~ColorButton();

	QColor color() const {return pal.color(label->backgroundRole());}
	bool useNativeDialog() const {return !options.testFlag(QColorDialog::DontUseNativeDialog);}
	bool useAlphaChannel() const {return options.testFlag(QColorDialog::ShowAlphaChannel);}
	bool frameOnly() const {return frame;}

public slots:
	void setColor(const QColor & col);
	void setUseNativeDialog(bool yes) {if (yes) options &= ~QColorDialog::DontUseNativeDialog; else options |= QColorDialog::DontUseNativeDialog;}
	void setUseAlphaChannel(bool yes) {if (yes) options |= QColorDialog::ShowAlphaChannel; else options &= ~QColorDialog::ShowAlphaChannel;}
	void setFrameOnly(bool yes) {frame = yes; setFlat(frame); resizeEvent(0);}

private:
	void mousePressEvent(QMouseEvent * e);
	void mouseMoveEvent(QMouseEvent * e);
	void resizeEvent(QResizeEvent * );
	void dragEnterEvent(QDragEnterEvent * e);
	void dropEvent(QDropEvent * e);

	QFrame * label;
	QWidget * back;
	QPalette pal;
	QPoint pp;
	QMenu menu;
	QColorDialog::ColorDialogOptions options;
	bool frame;

private slots:
	void clicked();
	void copy() {QApplication::clipboard()->setText(color().name());}
	void paste() {QColor c(QApplication::clipboard()->text()); if (c.isValid()) setColor(c);}
	void mix();

signals:
	void colorChanged(QColor);

};

#endif // COLORBUTTON_H
