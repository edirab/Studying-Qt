 #ifndef ICONEDLABEL_H
#define ICONEDLABEL_H

#include <QLabel>
#include <QIcon>
#include <QHBoxLayout>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class IconedLabel: public QFrame
{
	Q_OBJECT
	Q_ENUMS(Direction)
	Q_PROPERTY(QString text READ text WRITE setText)
	Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
	Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
	Q_PROPERTY(Direction direction READ direction WRITE setDirection)
public:
	enum Direction {LeftToRight = 0, RightToLeft = 1, TopToBottom = 2, BottomToTop = 3};

	explicit IconedLabel(QWidget * parent = 0): QFrame(parent) {
		label_.setAlignment(Qt::AlignCenter);
		icon_.setAlignment(Qt::AlignCenter);
		size_ = QSize(16, 16);
		setDirection(LeftToRight);
	}

	QString text() const {return label_.text();}
	QIcon icon() const {return icon_.pixmap() == 0 ? QIcon() : QIcon(*icon_.pixmap());}
	QSize iconSize() const {return size_;}
	Direction direction() const {return dir_;}

protected:
	QLabel label_;
	QLabel icon_;
	QIcon sicon_;
	QSize size_;
	Direction dir_;
	
public slots:
	void setText(const QString & t) {label_.setText(t);}
	void setIcon(const QIcon & i) {sicon_ = i; icon_.setPixmap(i.pixmap(size_));}
	void setIconSize(const QSize & s) {size_ = s; icon_.setPixmap(sicon_.pixmap(size_));}
	void setDirection(Direction d) {
		dir_ = d;
		if (layout() != 0)
			delete layout();
		QLayout * lay = new QBoxLayout((QBoxLayout::Direction)dir_);
		lay->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
		lay->addWidget(&label_);
		lay->addWidget(&icon_);
		lay->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
		lay->setContentsMargins(0, 0, 0, 0);
		setLayout(lay);
		update();
	}

signals:
	
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // ICONEDLABEL_H
