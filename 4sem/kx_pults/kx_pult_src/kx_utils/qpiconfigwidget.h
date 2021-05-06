#ifndef QPICONFIGWIDGET_H
#define QPICONFIGWIDGET_H

#include "qpiconfig.h"
#include "qpiconfignewdialog.h"
#include "qpiconfigvaluewidget.h"
#include <QTreeWidget>
#include <QComboBox>
#include <QEvent>
#include <QBoxLayout>
#include <QAction>
#include <QMenu>


class UComboBox: public QComboBox
{
	Q_OBJECT
public:
	UComboBox(QWidget * parent = 0): QComboBox(parent) {connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChange(int)));}
private slots:
	void indexChange(int i) {emit currentIndexChanged(i, this);}
signals:
	void currentIndexChanged(int, UComboBox * );
};


class QPIConfigWidget: public QTreeWidget
{
	Q_OBJECT
	Q_PROPERTY(bool readOnlyName READ readOnlyName WRITE setReadOnlyName)
	Q_PROPERTY(bool readOnlyValue READ readOnlyValue WRITE setReadOnlyValue)
	Q_PROPERTY(bool readOnlyType READ readOnlyType WRITE setReadOnlyType)
	Q_PROPERTY(bool readOnlyComment READ readOnlyComment WRITE setReadOnlyComment)
	Q_PROPERTY(bool columnNameVisible READ columnNameVisible WRITE setColumnNameVisible)
	Q_PROPERTY(bool columnValueVisible READ columnValueVisible WRITE setColumnValueVisible)
	Q_PROPERTY(bool columnTypeVisible READ columnTypeVisible WRITE setColumnTypeVisible)
	Q_PROPERTY(bool columnCommentVisible READ columnCommentVisible WRITE setColumnCommentVisible)
public:
	QPIConfigWidget(QWidget * parent = 0, QPIConfig * c = 0, bool on = true);
	~QPIConfigWidget() {clear();}
	
	void setQPIConfig(QPIConfig * c) {conf = c; buildTree();}
	bool readOnlyName() {return read_only_name;}
	bool readOnlyValue() {return read_only_value;}
	bool readOnlyType() {return read_only_type;}
	bool readOnlyComment() {return read_only_comment;}
	bool columnNameVisible() {return !c_hidden[0];}
	bool columnValueVisible() {return !c_hidden[1];}
	bool columnTypeVisible() {return !c_hidden[2];}
	bool columnCommentVisible() {return !c_hidden[3];}
	QString writeToString();
	void readFromString(QString str);
	
private:
	void changeEvent(QEvent * e);
	bool eventFilter(QObject * o, QEvent * e);
	void buildEntry(QTreeWidgetItem * i, QPIConfig::Entry * e);
	void buildFullNames(QTreeWidgetItem * i);
	QPIConfig::Entry * itemEntry(QTreeWidgetItem * i);
	ConfigValueWidget * itemCWidget(QTreeWidgetItem * i) {return qobject_cast<ConfigValueWidget * >(itemWidget(i, 1));}
	UComboBox * itemTWidget(QTreeWidgetItem * i) {return qobject_cast<UComboBox * >(itemWidget(i, 2));}
	QTreeWidgetItem * addEntry(QTreeWidgetItem * i, QPIConfig::Entry * e, bool node = false);
	void deleteEntry(QTreeWidgetItem * i);
	bool filter(const QString & f, QTreeWidgetItem * i);
	bool filterItem(const QString & f, QTreeWidgetItem * i);
	void translate();
	void addTrEntry(const QString & s, const QString & f) {types.insert(s, f); s_types << f;}
	
	QPIConfig * conf;
	QPIConfigNewDialog new_dialog;
	QAction actionAddItem, actionAddNode, actionToItem, actionToNode, actionRemove, actionExpandAll, actionCollapseAll;
	QMenu popupMenu;
	QString c_name, c_comment;
	QTreeWidgetItem * pi, * ti, * c_pi;
	QHash<QString, QString> types;
	QStringList s_types;
	QVector<ConfigValueWidget * > w_values;
	QVector<UComboBox * > w_types;
	QVector<bool> c_hidden;
	bool active, read_only_name, read_only_value, read_only_type, read_only_comment;
	
public slots:
	void parse() {if (conf == 0) clear(); else conf->readAll();}
	void write() {if (conf == 0) return; conf->buildFullNames(&(conf->root)); conf->writeAll();}
	void clear();
	void buildTree();
	void filter(const QString & f) {if (!active) return; filter(f, invisibleRootItem());}
	void setReadOnlyName(bool yes) {read_only_name = yes;}
	void setReadOnlyValue(bool yes);
	void setReadOnlyType(bool yes);
	void setReadOnlyComment(bool yes) {read_only_comment = yes;}
	void setColumnNameVisible(bool yes) {setColumnHidden(0, !yes); c_hidden[0] = !yes;}
	void setColumnValueVisible(bool yes) {setColumnHidden(1, !yes); c_hidden[1] = !yes;}
	void setColumnTypeVisible(bool yes) {setColumnHidden(2, !yes); c_hidden[2] = !yes;}
	void setColumnCommentVisible(bool yes) {setColumnHidden(3, !yes); c_hidden[3] = !yes;}
	
private slots:
	void itemClicked(QTreeWidgetItem * item, int column);
	void itemChanged(QTreeWidgetItem * item, int column);
	void typeChange(int t, UComboBox * c);
	void valueChange(ConfigValueWidget * w, QString v);
	void on_actionAddItem_triggered();
	void on_actionAddNode_triggered();
	void on_actionRemove_triggered();
	
signals:
	void changed();
	
};

#endif // QPICONFIGWIDGET_H
