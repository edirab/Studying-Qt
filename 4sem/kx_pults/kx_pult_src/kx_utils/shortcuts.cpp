#include "shortcuts.h"


void ShortcutEdit::keyPressEvent(QKeyEvent * e) {
	Qt::KeyboardModifiers km = e->modifiers();
	km &= ~Qt::KeypadModifier;
	km &= ~Qt::GroupSwitchModifier;
	if (e->key() != Qt::Key_Control && e->key() != Qt::Key_Shift &&
		e->key() != Qt::Key_Alt && e->key() != Qt::Key_Meta)
		setText(QKeySequence(km | e->key()).toString());
}



Shortcuts::Shortcuts(QWidget * parent, bool on): QTreeWidget(parent) {
	aw = 0;
	QImage ti(QSize(16, 16), QImage::Format_ARGB32_Premultiplied);
	QPainter p(&ti);
	p.setCompositionMode(QPainter::CompositionMode_Clear);
	p.eraseRect(ti.rect());
	p.end();
	empty_icon = QPixmap::fromImage(ti);
	bfont = font();
	bfont.setWeight(QFont::Bold);
	active = on;
	setColumnCount(2);
#if QT_VERSION < 0x050000
	header()->setResizeMode(0, QHeaderView::ResizeToContents);
	header()->setResizeMode(1, QHeaderView::ResizeToContents);
#else
	header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
#endif
	header()->setStretchLastSection(true);
	setColumnWidth(1, 200);
	setSortingEnabled(true);
	QStringList l;
	l << tr("Command") << tr("Shortcut");
	setHeaderLabels(l);
	assignWindow(parent);
}


Shortcuts::~Shortcuts() {
	foreach (ShortcutEdit * i, edits)
		delete i;
	edits.clear();
}


void Shortcuts::changeEvent(QEvent * e) {
	if (e->type() == QEvent::LanguageChange) {
		QStringList l;
		l << tr("Command") << tr("Shortcut");
		setHeaderLabels(l);
	}
}


void Shortcuts::assignWindow(QWidget * w) {
	if (w == 0) return;
	while ((qobject_cast<QMainWindow * >(w) == 0) && (w->parentWidget() != 0))
		w = w->parentWidget();
	aw = qobject_cast<QMainWindow * >(w);
	updateShortcuts();
}


QStringList Shortcuts::actionTree(QAction * a) {
	QStringList tree;
	QList<QWidget * > aw = a->associatedWidgets();
	if (aw.size() == 0) return tree;
	QWidget * cw = 0;
	QMenu * tm;
	QToolBar * tt;
	foreach (QWidget * i, aw) {
		tm = qobject_cast<QMenu * >(i);
		if (tm == 0) continue;
		cw = i;
		while (cw != 0) {
			tm = qobject_cast<QMenu * >(cw);
			if (tm != 0) {
				if (!tm->title().isEmpty())
					tree.push_front(tm->title());
				cw = cw->parentWidget();
			} else break;
		}
		if (!tree.isEmpty()) return tree;
	}
	foreach (QWidget * i, aw) {
		tt = qobject_cast<QToolBar * >(i);
		if (tt == 0) continue;
		cw = i;
		if (!tt->windowTitle().isEmpty())
			tree.push_front(tt->windowTitle());
		break;
	}
	return tree;
}


QList<QPair<QString, QKeySequence> > Shortcuts::shortcuts() {
	QList<QPair<QString, QKeySequence> > l;
	foreach (ShortcutEdit * i, edits) {
		if (i->action()->objectName().isEmpty()) continue;
		l << QPair<QString, QKeySequence>(i->action()->objectName(), i->text());
	}
	return l;
}


void Shortcuts::clear() {
	foreach (ShortcutEdit * i, edits)
		delete i;
	edits.clear();
	hide();
	QList<QTreeWidgetItem * > tl = findItems("", Qt::MatchContains);
	foreach (QTreeWidgetItem * i, tl)
		delete i;
	show();
}


bool Shortcuts::checkAction(QAction * a) {
	if (a->menu() != 0) return false;
	if (a->isSeparator()) return false;
	if (a->text().isEmpty()) return false;
	if (a->associatedWidgets().isEmpty()) return false;
	if (QString(a->metaObject()->className()) != "QAction") return false;
	if (qobject_cast<QWidgetAction * >(a) != 0) return false;
	return true;
}


void Shortcuts::updateShortcuts() {
	//return;
	if (aw == 0 || !active) return;
	hide();
	int cpos = verticalScrollBar()->value();
	clear();
#if QT_VERSION < 0x050000
	header()->setResizeMode(0, QHeaderView::Fixed);
#else
	header()->setSectionResizeMode(0, QHeaderView::Fixed);
#endif
	QList<QAction * > al = aw->findChildren<QAction * >();
	QTreeWidgetItem * pi, * ci;
	QStringList tree;
	bool s = isSortingEnabled(), isFound;
	setSortingEnabled(false);
	foreach (QAction * i, al) {
		if (!checkAction(i)) continue;
		edits.push_back(new ShortcutEdit());
		tree = actionTree(i);
		pi = invisibleRootItem();
		foreach (QString t, tree) {
			isFound = false;
			for (int j = 0; j < pi->childCount(); ++j) {
				if (pi->child(j)->text(0) == t) {
					pi = pi->child(j);
					isFound = true;
					break;
				}
			}
			if (isFound) continue;
			ci = new QTreeWidgetItem(pi);
			ci->setText(0, t);
			ci->setToolTip(0, t);
			ci->setFont(0, bfont);
			pi->addChild(ci);
			pi = ci;
		}
		ci = new QTreeWidgetItem(pi);
		ci->setText(0, i->text());
		ci->setToolTip(0, i->text());
		if (i->icon().isNull())
			ci->setIcon(0, empty_icon);
		else
			ci->setIcon(0, i->icon());
		edits.back()->ti = ci;
		edits.back()->assignAction(i);
		pi->addChild(ci);
		//qDebug() << "set widget" << edits.back();
		setItemWidget(ci, 1, edits.back());
	}
	setSortingEnabled(s);
#if QT_VERSION < 0x050000
	header()->setResizeMode(0, QHeaderView::ResizeToContents);
#else
	header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
#endif
	expandAll();
	verticalScrollBar()->setValue(cpos);
	show();
}


void Shortcuts::commit() {
	foreach (ShortcutEdit * i, edits)
		i->commit();
}


void Shortcuts::reset() {
	foreach (ShortcutEdit * i, edits)
		i->reset();
	updateShortcuts();
}


void Shortcuts::filter(const QString & what) {
	hide();
	for (int i = 0; i < topLevelItemCount(); ++i)
		filterTree(topLevelItem(i), what);
	show();
}


bool Shortcuts::filterTree(QTreeWidgetItem * ti, QString f) {
	if (f.isEmpty()) {
		for (int i = 0; i < ti->childCount(); ++i)
			filterTree(ti->child(i), f);
		ti->setHidden(false);
		return true;
	}
	bool isFound = false;
	for (int i = 0; i < ti->childCount(); ++i)
		if (filterTree(ti->child(i), f)) isFound = true;
	if (ti->text(0).indexOf(f, 0, Qt::CaseInsensitive) >= 0 ||
		ti->text(1).indexOf(f, 0, Qt::CaseInsensitive) >= 0) isFound = true;
	ti->setHidden(!isFound);
	return isFound;
}
