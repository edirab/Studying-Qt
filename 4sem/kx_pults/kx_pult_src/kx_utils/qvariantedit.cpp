#include "qvariantedit.h"
#include <QDateTimeEdit>
#include <QFileDialog>


StringListEdit::~StringListEdit() {
	delete combo;
	delete butt_add;
	delete butt_del;
	delete butt_clear;
}


QStringList StringListEdit::value() const {
	QStringList l;
	for (int i = 0; i < combo->count(); ++i) l << combo->itemText(i);
	return l;
}


void StringListEdit::setValue(const QStringList & v) {
	combo->clear();
	if (!v.isEmpty()) {
		combo->addItems(v);
		combo->setCurrentIndex(0);
	}
}


void StringListEdit::addItem() {
	combo->addItem(combo->currentText());
	emit valueChanged();
}


void StringListEdit::delItem() {
	if (combo->currentIndex() < 0) return;
	combo->removeItem(combo->currentIndex());
	emit valueChanged();
}


void StringListEdit::clear() {
	if (combo->count() == 0) return;
	combo->clear();
	emit valueChanged();
}


StringListEdit::StringListEdit(QWidget * parent): QWidget(parent), lay(QBoxLayout::LeftToRight, this) {
	combo = new EComboBox(this);
	combo->setEditable(true);
	combo->setLineEdit(new CLineEdit);
	combo->setInsertPolicy(QComboBox::NoInsert);
	butt_add = new QPushButton(this);
	butt_del = new QPushButton(this);
	butt_clear = new QPushButton(this);
	/*butt_add->setIconSize(QSize(16, 16));
	butt_del->setIconSize(QSize(16, 16));
	butt_clear->setIconSize(QSize(16, 16));*/
	butt_add->setMaximumWidth(combo->height());
	butt_del->setMaximumWidth(combo->height());
	butt_clear->setMaximumWidth(combo->height());
	butt_add->setIcon(QIcon(":/icons/list-add.png"));
	butt_del->setIcon(QIcon(":/icons/edit-delete.png"));
	butt_clear->setIcon(QIcon(":/icons/edit-clear.png"));
	butt_add->setToolTip(tr("Add"));
	butt_del->setToolTip(tr("Remove"));
	butt_clear->setToolTip(tr("Clear"));
	lay.setContentsMargins(0, 0, 0, 0);
	lay.setSpacing(2);
	lay.addWidget(combo);
	lay.addWidget(butt_add);
	lay.addWidget(butt_del);
	lay.addWidget(butt_clear);
	connect(combo->lineEdit(), SIGNAL(returnPressed()), this, SLOT(editItem()));
	connect(butt_add, SIGNAL(clicked(bool)), this, SLOT(addItem()));
	connect(butt_del, SIGNAL(clicked(bool)), this, SLOT(delItem()));
	connect(butt_clear, SIGNAL(clicked(bool)), this, SLOT(clear()));
}


void StringListEdit::changeEvent(QEvent * e) {
	if (e->type() == QEvent::LanguageChange) {
		butt_add->setToolTip(tr("Add"));
		butt_del->setToolTip(tr("Remove"));
		butt_clear->setToolTip(tr("Clear"));
		return;
	}
	QWidget::changeEvent(e);
}


void StringListEdit::editItem() {
	int ci = combo->currentIndex();
	if (ci < 0) return;
	combo->setItemText(ci, combo->currentText());
}




PathEdit::PathEdit(QWidget * parent): QWidget(parent), lay(QBoxLayout::LeftToRight, this) {
	is_dir = is_abs = false;
	filter = tr("All files(*)");
	line = new CLineEdit(this);
	butt_select = new QPushButton(this);
	//butt_select->setIconSize(QSize(16, 16));
	butt_select->setMaximumWidth(line->height());
	butt_select->setIcon(QIcon(":/icons/document-open.png"));
	butt_select->setToolTip(tr("Choose") + " ...");
	lay.setContentsMargins(0, 0, 0, 0);
	lay.setSpacing(2);
	lay.addWidget(line);
	lay.addWidget(butt_select);
	connect(line, SIGNAL(textChanged(QString)), this, SIGNAL(valueChanged()));
	connect(butt_select, SIGNAL(clicked(bool)), this, SLOT(select()));
}


void PathEdit::changeEvent(QEvent * e) {
	if (e->type() == QEvent::LanguageChange) {
		butt_select->setToolTip(tr("Choose") + " ...");
		return;
	}
	QWidget::changeEvent(e);
}


void PathEdit::select() {
	QString ret;
	if (is_dir) ret = QFileDialog::getExistingDirectory(this, tr("Select directory"), value());
	else ret = QFileDialog::getOpenFileName(this, tr("Select file"), value(), filter);
	if (ret.isEmpty()) return;
	if (!is_abs)
		ret = QDir::current().relativeFilePath(ret);
	line->setText(ret);
	emit valueChanged();
}




QVariantEdit::QVariantEdit(QWidget * parent): QWidget(parent) {
	_empty = 0;
	_line = 0;
	_check = 0;
	_color = 0;
	_list = 0;
	_date = 0;
	_spin = 0;
	_rect = 0;
	_point = 0;
	_path = 0;
	_enum = 0;
	_cur_edit = 0;
	_recreate(QVariant());
}


QVariantEdit::~QVariantEdit() {
	_delete();
}


void QVariantEdit::resizeEvent(QResizeEvent * e) {
	_resize();
	QWidget::resizeEvent(e);
}


void QVariantEdit::_recreate(const QVariant & new_value) {
	if (!new_value.isValid()) {
		if (_cur_edit != _empty) _delete();
		if (_empty == 0) {
			_empty = new QLabel(trUtf8("Invalid value"), this);
			_empty->setAlignment(Qt::AlignCenter);
			_cur_edit = _empty;
			_resize();
		}
		_value = new_value;
		return;
	}
	if (_value.userType() == new_value.userType()) {
		_value = new_value;
		return;
	}
	_delete();
	switch (new_value.type()) {
	case QVariant::Bool:
		_check = new QCheckBox(this);
		_check->setAutoFillBackground(true);
		_cur_edit = _check;
		connect(_check, SIGNAL(toggled(bool)), this, SLOT(_changed()));
		break;
	case QVariant::Int:
		_spin = new QDoubleSpinBox(this);
		_spin->setDecimals(0);
		_spin->setRange(-0x7FFFFFFF, 0x7FFFFFFF);
		_cur_edit = _spin;
		connect(_spin, SIGNAL(valueChanged(double)), this, SLOT(_changed()));
		break;
	case QVariant::UInt:
		_spin = new QDoubleSpinBox(this);
		_spin->setDecimals(0);
		_spin->setRange(0, 0xFFFFFFFF);
		_cur_edit = _spin;
		connect(_spin, SIGNAL(valueChanged(double)), this, SLOT(_changed()));
		break;
	case QVariant::LongLong:
		_spin = new QDoubleSpinBox(this);
		_spin->setDecimals(0);
		_spin->setRange(-0x7FFFFFFFFFFFFFFFL, 0x7FFFFFFFFFFFFFFFL);
		_cur_edit = _spin;
		connect(_spin, SIGNAL(valueChanged(double)), this, SLOT(_changed()));
		break;
	case QVariant::ULongLong:
		_spin = new QDoubleSpinBox(this);
		_spin->setDecimals(0);
		_spin->setRange(0L, 0x7FFFFFFFFFFFFFFFL);
		_cur_edit = _spin;
		connect(_spin, SIGNAL(valueChanged(double)), this, SLOT(_changed()));
		break;
	case QVariant::Double:
		_spin = new QDoubleSpinBox(this);
		_spin->setDecimals(5);
		_spin->setRange(-1E+199, 1E+199);
		_cur_edit = _spin;
		connect(_spin, SIGNAL(valueChanged(double)), this, SLOT(_changed()));
		break;
	case QVariant::Color:
		_color = new ColorButton(this);
		_color->setUseAlphaChannel(true);
		_cur_edit = _color;
		connect(_color, SIGNAL(colorChanged(QColor)), this, SLOT(_changed()));
		break;
	case QVariant::String:
		_line = new CLineEdit(this);
		_cur_edit = _line;
		connect(_line, SIGNAL(textChanged(QString)), this, SLOT(_changed()));
		break;
	case QVariant::StringList:
		_list = new StringListEdit(this);
		_cur_edit = _list;
		connect(_list, SIGNAL(valueChanged()), this, SLOT(_changed()));
		break;
	case QVariant::Rect:
		_rect = new QRectEdit(this);
		_rect->setDecimals(0);
		_cur_edit = _rect;
		connect(_rect, SIGNAL(valueChanged(QRectF)), this, SLOT(_changed()));
		break;
	case QVariant::RectF:
		_rect = new QRectEdit(this);
		_rect->setDecimals(3);
		_cur_edit = _rect;
		connect(_rect, SIGNAL(valueChanged(QRectF)), this, SLOT(_changed()));
		break;
	case QVariant::Point:
		_point = new QPointEdit(this);
		_point->setDecimals(0);
		_cur_edit = _point;
		connect(_point, SIGNAL(valueChanged(QPointF)), this, SLOT(_changed()));
		break;
	case QVariant::PointF:
		_point = new QPointEdit(this);
		_point->setDecimals(3);
		_cur_edit = _point;
		connect(_point, SIGNAL(valueChanged(QPointF)), this, SLOT(_changed()));
		break;
	case QVariant::Date:
		_date = new QDateEdit(this);
		_cur_edit = _date;
		connect(_date, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(_changed()));
		break;
	case QVariant::Time:
		_date = new QTimeEdit(this);
		_cur_edit = _date;
		connect(_date, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(_changed()));
		break;
	case QVariant::DateTime:
		_date = new QDateTimeEdit(this);
		_cur_edit = _date;
		connect(_date, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(_changed()));
		break;
	default: break;
	}
	if (!_cur_edit) {
		if (new_value.canConvert<QAD::Enum>()) {
			_enum = new EComboBox(this);
			_setEnum(new_value.value<QAD::Enum>());
			_cur_edit = _enum;
			connect(_enum, SIGNAL(currentIndexChanged(int)), this, SLOT(_changed()));
		}
		if (new_value.canConvert<QAD::File>()) {
			_path = new PathEdit(this);
			_setFile(new_value.value<QAD::File>());
			_cur_edit = _path;
			connect(_path, SIGNAL(valueChanged()), this, SLOT(_changed()));
		}
		if (new_value.canConvert<QAD::Dir>()) {
			_path = new PathEdit(this);
			_setDir(new_value.value<QAD::Dir>());
			_cur_edit = _path;
			connect(_path, SIGNAL(valueChanged()), this, SLOT(_changed()));
		}
	}
	//qDebug() << _cur_edit;
	if (_cur_edit) {
		_resize();
		_cur_edit->show();
	}
	_value = new_value;
}


QVariant QVariantEdit::value() const {
	switch (_value.type()) {
	case QVariant::Bool: return _check->isChecked();
	case QVariant::Int: return int(_spin->value());
	case QVariant::UInt: return (unsigned int)(_spin->value());
	case QVariant::LongLong: return qlonglong(_spin->value());
	case QVariant::ULongLong: return qulonglong(_spin->value());
	case QVariant::Double: return double(_spin->value());
	case QVariant::Color: return _color->color();
	case QVariant::String: return _line->text();
	case QVariant::StringList: return _list->value();
	case QVariant::Rect: return _rect->value().toRect();
	case QVariant::RectF: return _rect->value();
	case QVariant::Point: return _point->value().toPoint();
	case QVariant::PointF: return _point->value();
	case QVariant::Date: return _date->date();
	case QVariant::Time: return _date->time();
	case QVariant::DateTime: return _date->dateTime();
	default:
		if (_value.canConvert<QAD::Enum>() && _enum) {
			QAD::Enum ret;
			for (int i = 0; i < _enum->count(); ++i)
				ret.enum_list << QAD::Enumerator(_enum->itemData(i).toInt(), _enum->itemText(i));
			ret.enum_name = _enum->property("enum_name").toString();
			ret.selected = _enum->currentText();
			return QVariant::fromValue<QAD::Enum>(ret);
		}
		if (_value.canConvert<QAD::File>() && _path) {
			if (!_path->is_dir) {
				QAD::File ret;
				ret.file = _path->value();
				ret.filter = _path->filter;
				ret.is_abs = _path->is_abs;
				return QVariant::fromValue<QAD::File>(ret);
			}
		}
		if (_value.canConvert<QAD::Dir>() && _path) {
			if (_path->is_dir) {
				QAD::Dir ret;
				ret.dir = _path->value();
				ret.is_abs = _path->is_abs;
				return QVariant::fromValue<QAD::Dir>(ret);
			}
		}
	}
	return QVariant();
}


void QVariantEdit::setValue(const QVariant & v) {
	_recreate(v);
	if (_cur_edit) _cur_edit->blockSignals(true);
	if (_line) {_line->setText(v.toString());}
	if (_check) {_check->setChecked(v.toBool()); _check->setText(v.toBool() ? "true" : "false");}
	if (_color) {_color->setColor(v.value<QColor>());}
	if (_list) {_list->setValue(v.toStringList());}
	if (_date) {_date->setDateTime(v.toDateTime());}
	if (_spin) {_spin->setValue(v.toDouble());}
	if (_rect) {_rect->setValue(v.toRectF());}
	if (_point) {_point->setValue(v.toPointF());}
	if (_path) {
		if (_path->is_dir) _setDir(v.value<QAD::Dir>());
		else _setFile(v.value<QAD::File>());
	}
	if (_enum) {_setEnum(v.value<QAD::Enum>());}
	if (_cur_edit) _cur_edit->blockSignals(false);
}


void QVariantEdit::_delete() {
	if (_cur_edit)
		delete _cur_edit;
	_cur_edit = 0;
	_empty = 0;
	_line = 0;
	_check = 0;
	_color = 0;
	_list = 0;
	_date = 0;
	_spin = 0;
	_rect = 0;
	_point = 0;
	_path = 0;
	_enum = 0;
}


void QVariantEdit::_newPath() {
	_delete();
	_path = new PathEdit(this);
	_cur_edit = _path;
	_value = _value.toString();
	connect(_path, SIGNAL(valueChanged()), this, SLOT(_changed()));
	_resize();
	_cur_edit->show();
}


void QVariantEdit::_setEnum(const QAD::Enum & v) {
	_enum->clear();
	_enum->setProperty("enum_name", v.enum_name);
	foreach (const QAD::Enumerator & e, v.enum_list)
		_enum->addItem(e.name, QVariant(e.value));
	int i(0);
	for (i = 0; i < _enum->count(); ++i)
		if (_enum->itemText(i) == v.selected) {
			_enum->setCurrentIndex(i);
			break;
		}
	if (i == _enum->count())
		_enum->setCurrentIndex(-1);
}


void QVariantEdit::_setFile(const QAD::File & v) {
	_path->is_dir = false;
	_path->setValue(v.file);
	_path->filter = v.filter;
	_path->is_abs = v.is_abs;
}


void QVariantEdit::_setDir(const QAD::Dir & v) {
	_path->is_dir = true;
	_path->setValue(v.dir);
	_path->is_abs = v.is_abs;
}

void QVariantEdit::_changed() {
	if (_check) _check->setText(_check->isChecked() ? "true" : "false");
	emit valueChanged(value());
}

