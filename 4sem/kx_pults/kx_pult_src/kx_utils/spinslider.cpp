#include "spinslider.h"


SpinSlider::SpinSlider(QWidget * parent): QWidget(parent) {
	min_ = val_ = 0.;
	max_ = 100.;
	dec_ = 1;
	page = 10.;
	ticks_ = 1;
	direc = LeftToRight;
	square = false;
	slider = new QSlider();
	slider->setOrientation(Qt::Horizontal);
	slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	spin = new QDoubleSpinBox();
	adjust();
	layout = new QBoxLayout(QBoxLayout::LeftToRight);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(slider);
	layout->addWidget(spin);
	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
	connect(spin, SIGNAL(valueChanged(double)), this, SLOT(spinChanged(double)));
	setLayout(layout);
}


SpinSlider::~SpinSlider() {
	delete spin;
	delete slider;
	delete layout;
}


void SpinSlider::setOrientation(Qt::Orientation orient) {
	slider->setOrientation(orient);
	if (orient == Qt::Horizontal)
		slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	else
		slider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}


void SpinSlider::adjust() {
	adjusting = true;
	delim = qPow(10, dec_);
	spin->setDecimals(dec_);
	spin->setRange(min_, max_);
	if (square) slider->setRange(sqrt(min_ * delim), sqrt(max_ * delim));
	else slider->setRange(min_ * delim, max_ * delim);
	if (val_ < min_) val_ = min_;
	if (val_ > max_) val_ = max_;
	spin->setValue(val_);
	if (square) slider->setValue(static_cast<int>(sqrt(val_ * delim)));
	else slider->setValue(static_cast<int>(val_ * delim));
	slider->setPageStep(qRound(page * delim));
	slider->setTickInterval(qRound(ticks_ * delim));
	emit valueChanged(val_);
	emit valueChanged(qRound(val_));
	adjusting = false;
}


void SpinSlider::sliderChanged(int value) {
	if (adjusting) return;
	adjusting = true;
	if (square) spin->setValue(static_cast<double>(sqr(value) / delim));
	else spin->setValue(static_cast<double>(value) / delim);
	val_ = spin->value();
	emit valueChanged(val_);
	emit valueChanged(qRound(val_));
	adjusting = false;
}


void SpinSlider::spinChanged(double value) {
	if (adjusting) return;
	adjusting = true;
	val_ = value;
	if (square) slider->setValue(static_cast<int>(sqrt(value * delim)));
	else slider->setValue(qRound(value * delim));
	emit valueChanged(val_);
	emit valueChanged(qRound(val_));
	adjusting = false;
}
