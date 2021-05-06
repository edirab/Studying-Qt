#ifndef QPICONFIGVALUEWIDGET_H
#define QPICONFIGVALUEWIDGET_H

#include "qpiconfig.h"
#include "qvariantedit.h"
#include "qrectedit.h"
#include "qpointedit.h"
#include "colorbutton.h"
#include "ecombobox.h"
#include "qipedit.h"
#include "limits.h"
#include "float.h"
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>


class ConfigValueWidget: public QWidget
{
	Q_OBJECT
	friend class QPIConfigWidget;
	friend class QPIConfigNewDialog;
public:
	ConfigValueWidget(QWidget * parent = 0);
	~ConfigValueWidget() {hide();}
	
	void setType(const QString & t);
	void setValue(const QString & v);
	void setEntry(QPIConfig::Entry * e) {value = e->value(); full_name = e->_full_name; comment = e->comment(); setType(e->type());}
	
private:
	void hideAll() {w_string.hide(); w_list.hide(); w_bool.hide(); w_integer.hide(); w_float.hide(); w_color.hide(); w_rect.hide(); w_point.hide(); w_ip.hide(); w_enum.hide(); w_path.hide();}
	
	QString type, value, full_name, comment;
	bool active;
	QBoxLayout lay;
	CLineEdit w_string;
	StringListEdit w_list;
	ColorButton w_color;
	QCheckBox w_bool;
	QSpinBox w_integer;
	QDoubleSpinBox w_float;
	QRectEdit w_rect;
	QPointEdit w_point;
	QIPEdit w_ip;
	QComboBox w_enum;
	PathEdit w_path;
	
private slots:
	void valueChanged();
	
signals:
	void changed(ConfigValueWidget * , QString );
	
};

#endif // QPICONFIGVALUEWIDGET_H
