#ifndef SPINSLIDER_H
#define SPINSLIDER_H

#include <QSlider>
#include <QDoubleSpinBox>
#include <QBoxLayout>
#include <qmath.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class SpinSlider: public QWidget
{
	Q_OBJECT
	Q_ENUMS(Direction)
	Q_PROPERTY(double minimum READ minimum WRITE setMinimum)
	Q_PROPERTY(double maximum READ maximum WRITE setMaximum)
	Q_PROPERTY(double value READ value WRITE setValue)
	Q_PROPERTY(int decimals READ decimals WRITE setDecimals)
	Q_PROPERTY(double singleStep READ singleStep WRITE setSingleStep)
	Q_PROPERTY(double pageStep READ pageStep WRITE setPageStep)
	Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
	Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
	Q_PROPERTY(QSlider::TickPosition tickPosition READ tickPosition WRITE setTickPosition)
	Q_PROPERTY(int tickInterval READ tickInterval WRITE setTickInterval)
	Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
	Q_PROPERTY(Direction direction READ direction WRITE setDirection)
	Q_PROPERTY(bool invertedAppearance READ invertedAppearance WRITE setInvertedAppearance)
	Q_PROPERTY(bool squareScale READ squareScale WRITE setSquareScale)
	Q_PROPERTY(double spinMinimum READ spinMinimum WRITE setSpinMinimum)
	Q_PROPERTY(double spinMaximum READ spinMaximum WRITE setSpinMaximum)

public:
	explicit SpinSlider(QWidget * parent = 0);
	~SpinSlider();

	enum Direction {LeftToRight, RightToLeft, TopToBottom, BottomToTop};

	double minimum() const {return min_;}
	double maximum() const {return max_;}
	double spinMinimum() const {return spin->minimum();}
	double spinMaximum() const {return spin->maximum();}
	double value() const {return val_;}
	int decimals() const {return dec_;}
	double singleStep() const {return spin->singleStep();}
	double pageStep() const {return page;}
	QString prefix() const {return spin->prefix();}
	QString suffix() const {return spin->suffix();}
	QSlider::TickPosition tickPosition() const {return slider->tickPosition();}
	int tickInterval() const {return ticks_;}
	Qt::Orientation orientation() const {return slider->orientation();}
	Direction direction() const {return direc;}
	bool invertedAppearance() const {return slider->invertedAppearance();}
	bool squareScale() const {return square;}

    void setSingleStep(double step) {spin->setSingleStep(step); slider->setPageStep(qRound(step * delim));}
    void setPageStep(double step) {page = step; slider->setPageStep(qRound(page * delim));}
	void setPrefix(QString prefix) {spin->setPrefix(prefix);}
	void setSuffix(QString suffix) {spin->setSuffix(suffix);}
	void setTickPosition(QSlider::TickPosition tp) {slider->setTickPosition(tp);}
    void setTickInterval(int ti) {ticks_ = ti; slider->setTickInterval(qRound(ticks_ * delim));}
	void setOrientation(Qt::Orientation orient);
	void setDirection(Direction d) {direc = d; layout->setDirection((QBoxLayout::Direction)d);}
	void setInvertedAppearance(bool yes) {slider->setInvertedAppearance(yes);}
	void setSquareScale(bool yes) {square = yes; adjust();}

public slots:
	void setMinimum(double value) {min_ = value; adjust();}
	void setMaximum(double value) {max_ = value; adjust();}
	void setSpinMinimum(double value) {spin->setMinimum(value);}
	void setSpinMaximum(double value) {spin->setMaximum(value);}
	void setValue(double value) {val_ = value; spin->setValue(value);}
	void setDecimals(int value) {dec_ = value; adjust();}
	void reset() {val_ = 0.; spin->setValue(0.);}

private:
	void adjust();
	double sqr(const double & v) {return v * v;}

	double min_, max_, val_, delim, page;
	int dec_, ticks_;
	bool adjusting, square;
	QSlider * slider;
	QDoubleSpinBox * spin;
	QBoxLayout * layout;
	Direction direc;

private slots:
	void sliderChanged(int value);
	void spinChanged(double value);

signals:
	void valueChanged(double);
	void valueChanged(int);

};

QT_END_NAMESPACE

QT_END_HEADER

#endif // SPINSLIDER_H
