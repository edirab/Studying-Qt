#ifndef QVARIANTEDIT_H
#define QVARIANTEDIT_H

#include "clineedit.h"
#include "ecombobox.h"
#include "colorbutton.h"
#include "qrectedit.h"
#include "qpointedit.h"
#include "qad_types.h"
#include <QCheckBox>
#include <QDoubleSpinBox>


class StringListEdit: public QWidget
{
	Q_OBJECT
public:
	StringListEdit(QWidget * parent = 0);
	~StringListEdit();
	
	QStringList value() const;
	
private:
	virtual void changeEvent(QEvent * e);
	
	QBoxLayout lay;
	EComboBox * combo;
	QPushButton * butt_add, * butt_del, * butt_clear;
	
public slots:
	void setValue(const QStringList & v);
	
private slots:
	void editItem();
	void addItem();
	void delItem();
	void clear();
	
signals:
	void valueChanged();
	
};




class PathEdit: public QWidget
{
	Q_OBJECT
public:
	PathEdit(QWidget * parent = 0);
	~PathEdit() {delete line; delete butt_select;}
	
	QString value() const {return line->text();}
	
	bool is_dir, is_abs;
	QString filter;
	
private:
	virtual void changeEvent(QEvent * e);
	
	QBoxLayout lay;
	CLineEdit * line;
	QPushButton * butt_select;
	
public slots:
	void setValue(const QString & v) {line->setText(v);}
	
private slots:
	void select();
	
signals:
	void valueChanged();
	
};




class QVariantEdit: public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QVariant value READ value WRITE setValue)
	
public:
	explicit QVariantEdit(QWidget * parent = 0);
	~QVariantEdit();
	
	QVariant value() const;
	QSize sizeHint() const {if (_cur_edit) return _cur_edit->sizeHint(); return QWidget::sizeHint();}
	
protected:
	void resizeEvent(QResizeEvent * );
	void _recreate(const QVariant & new_value);
	void _delete();
	void _resize() {if (_cur_edit) _cur_edit->resize(size());}
	void _newPath();
	void _setEnum(const QAD::Enum & v);
	void _setFile(const QAD::File & v);
	void _setDir(const QAD::Dir & v);
	
	
	QLabel * _empty;
	CLineEdit * _line;
	QCheckBox * _check;
	ColorButton * _color;
	StringListEdit * _list;
	QDateTimeEdit * _date;
	QDoubleSpinBox * _spin;
	QRectEdit * _rect;
	QPointEdit * _point;
	PathEdit * _path;
	EComboBox * _enum;
	QWidget * _cur_edit;
	QVariant _value;
	
private slots:
	void _changed();
	
public slots:
	void setValue(const QVariant & v);
	
signals:
	void valueChanged(QVariant);
	
};

#endif // QVARIANTEDIT_H
