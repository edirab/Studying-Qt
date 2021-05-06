#ifndef SHORTCUTS_H
#define SHORTCUTS_H

#include <QTreeWidget>
#include <QMainWindow>
#include <QShortcut>
#include <QHeaderView>
#include <QAction>
#include <QLineEdit>
#include <QDebug>
#include <QKeyEvent>
#include <QMenu>
#include <QToolBar>
#include <QScrollBar>
#include <QWidgetAction>
#include "clineedit.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class ShortcutEdit: public CLineEdit
{
	Q_OBJECT
	friend class Shortcuts;
public:
    explicit ShortcutEdit(QWidget * parent = 0): CLineEdit(parent) {ti = 0; ca = 0; connect(this, SIGNAL(textChanged(QString)), this, SLOT(textChanged_(QString)));}

	void assignAction(QAction * a) {clear(); ca = a; reset();}
	QAction * action() const {return ca;}
	bool isEmpty() const {return text().isEmpty();}
	void commit() {if (ca == 0) return; ca->setShortcut(QKeySequence(text()));}
	void reset() {if (ca == 0) return; setText(ca->shortcut().toString());}

private slots:
	void textChanged_(QString t) {if (ti != 0) ti->setText(1, t);}

private:
	void keyPressEvent(QKeyEvent * e);

	QAction * ca;
	QTreeWidgetItem * ti;

};


class Shortcuts: public QTreeWidget
{
	Q_OBJECT

public:
	explicit Shortcuts(QWidget * parent = 0, bool on = true);
	~Shortcuts();

	void assignWindow(QWidget * w);
	void setActive(bool on) {active = on;}
	QList<QPair<QString, QKeySequence> > shortcuts();
	QStringList actionTree(QAction * a);
	static bool checkAction(QAction * a);

public slots:
	void clear();
	void updateShortcuts();
	void commit();
	void reset();
	void filter(const QString & what);

private:
    virtual void updateEditorGeometries() {foreach (ShortcutEdit * i, edits) i->setGeometry(visualRect(indexFromItem(i->ti, 1)));}
	virtual void changeEvent(QEvent * );
	bool filterTree(QTreeWidgetItem * ti, QString f);

	QMainWindow * aw;
	QVector<ShortcutEdit * > edits;
	QIcon empty_icon;
	QFont bfont;
	bool active;

private slots:

signals:
	void shortcutChanged(QAction * , QShortcut & );

};

QT_END_NAMESPACE

QT_END_HEADER

#endif // SPINSLIDER_H
