#include "qpiconfigvaluewidget.h"
#include "qpievaluator.h"


ConfigValueWidget::ConfigValueWidget(QWidget * parent): QWidget(parent), lay(QBoxLayout::Down, this) {
	lay.setContentsMargins(0, 0, 0, 0);
	w_integer.setRange(INT_MIN, INT_MAX);
	w_float.setRange(-DBL_MAX, DBL_MAX);
	w_float.setDecimals(5);
	active = true;
	lay.addWidget(&w_string);
	lay.addWidget(&w_list);
	lay.addWidget(&w_bool);
	lay.addWidget(&w_integer);
	lay.addWidget(&w_float);
	lay.addWidget(&w_color);
	lay.addWidget(&w_rect);
	lay.addWidget(&w_point);
	lay.addWidget(&w_ip);
	lay.addWidget(&w_enum);
	lay.addWidget(&w_path);
	connect(&w_string, SIGNAL(textChanged(QString)), this, SLOT(valueChanged()));
	connect(&w_list, SIGNAL(valueChanged()), this, SLOT(valueChanged()));
	connect(&w_bool, SIGNAL(toggled(bool)), this, SLOT(valueChanged()));
	connect(&w_integer, SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
	connect(&w_float, SIGNAL(valueChanged(double)), this, SLOT(valueChanged()));
	connect(&w_color, SIGNAL(colorChanged(QColor)), this, SLOT(valueChanged()));
	connect(&w_rect, SIGNAL(valueChanged(QRectF)), this, SLOT(valueChanged()));
	connect(&w_point, SIGNAL(valueChanged(QPointF)), this, SLOT(valueChanged()));
	connect(&w_ip, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged()));
	connect(&w_enum, SIGNAL(currentIndexChanged(int)), this, SLOT(valueChanged()));
	connect(&w_path, SIGNAL(valueChanged()), this, SLOT(valueChanged()));
}


void ConfigValueWidget::setType(const QString & t) {
	hideAll();
	type = t.left(1);
	active = false;
	if (type == "e") {QStringList en_sl = QPIEvaluator::inBrackets(comment).split(';');
		if (en_sl.size()>1) {
		w_enum.show(); w_enum.addItems(en_sl); setValue(value); active = true; return;
		} else {type = "s";}}
	if (type == "s") {w_string.show(); setValue(value); active = true; return;}
	if (type == "l") {w_list.show(); setValue(value); active = true; return;}
	if (type == "b") {w_bool.show(); setValue(value); active = true; return;}
	if (type == "n") {w_integer.show(); setValue(value); active = true; return;}
	if (type == "f") {w_float.show(); setValue(value); active = true; return;}
	if (type == "c") {w_color.show(); setValue(value); active = true; return;}
	if (type == "r") {w_rect.show(); w_rect.setDecimals(0); setValue(value); active = true; return;}
	if (type == "a") {w_rect.show(); w_rect.setDecimals(3); setValue(value); active = true; return;}
	if (type == "p") {w_point.show(); w_point.setDecimals(0); setValue(value); active = true; return;}
	if (type == "v") {w_point.show(); w_point.setDecimals(3); setValue(value); active = true; return;}
	if (type == "i") {w_ip.show(); setValue(value); active = true; return;}
	if (type == "F") {w_path.show(); setValue(value); active = true; return;}
	if (type == "D") {w_path.show(); setValue(value); active = true; return;}
}


void ConfigValueWidget::setValue(const QString & v) {
	value = v;
	active = false;
	if (type == "l") {w_list.setValue(v.split("%|%")); active = true; return;}
	if (type == "b") {w_bool.setChecked(v.toInt() > 0 || v.toLower().trimmed() == "true"); active = true; return;}
	if (type == "n") {w_integer.setValue(QString2int(v)); active = true; return;}
	if (type == "f") {w_float.setValue(v.toDouble()); active = true; return;}
	if (type == "c") {w_color.setColor(QString2QColor(v)); active = true; return;}
	if (type == "r") {w_rect.setValue(QString2QRectF(v)); active = true; return;}
	if (type == "a") {w_rect.setValue(QString2QRectF(v)); active = true; return;}
	if (type == "p") {w_point.setValue(QString2QPointF(v)); active = true; return;}
	if (type == "v") {w_point.setValue(QString2QPointF(v)); active = true; return;}
	if (type == "i") {w_ip.setIP(v); active = true; return;}
	if (type == "e") {w_enum.setCurrentIndex(w_enum.findText(v)); active = true; return;}
	if (type == "F") {w_path.is_dir = false; w_path.setValue(v); active = true; return;}
	if (type == "D") {w_path.is_dir = true; w_path.setValue(v); active = true; return;}
	w_string.setText(v);
	active = true;
}


void ConfigValueWidget::valueChanged() {
	if (!active) return;
	if (type == "l") {value = w_list.value().join("%|%"); emit changed(this, value); return;}
	if (type == "b") {value = w_bool.isChecked() ? "true" : "false"; emit changed(this, value); return;}
	if (type == "n") {value = QString::number(w_integer.value()); emit changed(this, value); return;}
	if (type == "f") {value = QString::number(w_float.value()); emit changed(this, value); return;}
	if (type == "c") {value = QColor2QString(w_color.color()); emit changed(this, value); return;}
	if (type == "r") {value = QRectF2QString(w_rect.value()); emit changed(this, value); return;}
	if (type == "a") {value = QRectF2QString(w_rect.value()); emit changed(this, value); return;}
	if (type == "p") {value = QPointF2QString(w_point.value()); emit changed(this, value); return;}
	if (type == "v") {value = QPointF2QString(w_point.value()); emit changed(this, value); return;}
	if (type == "i") {value = w_ip.IP(); emit changed(this, value); return;}
	if (type == "e") {value = w_enum.currentText(); emit changed(this, value); return;}
	if (type == "F") {value = w_path.value(); emit changed(this, value); return;}
	if (type == "D") {value = w_path.value(); emit changed(this, value); return;}
	value = w_string.text();
	emit changed(this, value);
}
