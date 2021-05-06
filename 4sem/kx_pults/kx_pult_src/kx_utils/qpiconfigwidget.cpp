#include "qpiconfigwidget.h"


QPIConfigWidget::QPIConfigWidget(QWidget * parent, QPIConfig * c, bool on): QTreeWidget(parent), actionAddItem(this), actionAddNode(this),
																			actionToItem(this), actionToNode(this), actionRemove(this),
																			actionExpandAll(this), actionCollapseAll(this) {
	active = on;
	if (active) {
		setColumnCount(4);
		setColumnWidth(0, 150);
		setColumnWidth(1, 200);
	} else setColumnCount(0);
	setSelectionMode(ExtendedSelection);
	setVerticalScrollMode(ScrollPerPixel);
	actionAddItem.setIcon(QIcon(":/icons/item-add.png"));
	actionAddNode.setIcon(QIcon(":/icons/node-add.png"));
	actionToItem.setIcon(QIcon(":/icons/item.png"));
	actionToNode.setIcon(QIcon(":/icons/node.png"));
	actionRemove.setIcon(QIcon(":/icons/edit-delete.png"));
	popupMenu.addAction(&actionAddItem);
	popupMenu.addAction(&actionAddNode);
	popupMenu.addSeparator();
	/*popupMenu.addAction(&actionToItem);
	popupMenu.addAction(&actionToNode);
	popupMenu.addSeparator();*/
	popupMenu.addAction(&actionRemove);
	popupMenu.addSeparator();
	popupMenu.addAction(&actionExpandAll);
	popupMenu.addAction(&actionCollapseAll);
	viewport()->installEventFilter(this);
	connect(this, SIGNAL(itemClicked(QTreeWidgetItem * , int)), this, SLOT(itemClicked(QTreeWidgetItem * , int)));
	connect(this, SIGNAL(itemChanged(QTreeWidgetItem * , int)), this, SLOT(itemChanged(QTreeWidgetItem * , int)));
	connect(&actionAddItem, SIGNAL(triggered()), this, SLOT(on_actionAddItem_triggered()));
	connect(&actionAddNode, SIGNAL(triggered()), this, SLOT(on_actionAddNode_triggered()));
	connect(&actionRemove, SIGNAL(triggered()), this, SLOT(on_actionRemove_triggered()));
	connect(&actionExpandAll, SIGNAL(triggered()), this, SLOT(expandAll()));
	connect(&actionCollapseAll, SIGNAL(triggered()), this, SLOT(collapseAll()));
	read_only_name = read_only_value = read_only_type = read_only_comment = false;
	c_hidden << false << false << false << false;
	pi = c_pi = 0;
	translate();
	setQPIConfig(c);
	//resize(600, 400);
	//show();
}


void QPIConfigWidget::changeEvent(QEvent * e) {
	if (e->type() == QEvent::LanguageChange) {
		translate();
		return;
	}
	//for (int i = 0; i < 4; ++i)
	//	setColumnHidden(i, c_hidden[i]);
	QTreeWidget::changeEvent(e);
}


bool QPIConfigWidget::eventFilter(QObject * o, QEvent * e) {
	if (e->type() == QEvent::MouseButtonPress) {
		if (viewport() == qobject_cast<QWidget * >(o)) {
			pi = itemAt(((QMouseEvent * )e)->pos());
			if (((QMouseEvent * )e)->buttons() == Qt::RightButton) {
				qApp->processEvents();
				itemClicked(pi, 1);
				popupMenu.popup(((QMouseEvent * )e)->globalPos());
			}
		}
	}
	return QTreeWidget::eventFilter(o, e);
}


void QPIConfigWidget::itemClicked(QTreeWidgetItem * item, int column) {
	if (item != 0) {
		if ((column == 0 && !read_only_name) || (column == 3 && !read_only_comment)) item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
		else item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
	bool node = true, ro = read_only_name || read_only_type || read_only_value;
	if (item != 0)
		if (itemTWidget(item) != 0)
			if (itemTWidget(item)->isEnabled())
				node = false;
	actionAddItem.setVisible(node && !ro);
	actionAddNode.setVisible(node && !ro);
	actionRemove.setVisible(!ro && !selectedItems().isEmpty());
}


void QPIConfigWidget::itemChanged(QTreeWidgetItem * item, int column) {
	if (item != c_pi) {
		c_pi = item;
		if (item != 0) {
			c_name = item->text(0);
			c_comment = item->text(3);
		}
		return;
	}
	if (item == 0) return;
	if (c_name == item->text(0) && c_comment == item->text(3)) return;
	//qDebug() << "change" << item->text(0);
	QPIConfig::Entry * e = itemEntry(item);
	if (e == 0) return;
	if (column == 0) {
		buildFullNames(item);
		e->setName(item->text(column));
		conf->buildFullNames(e->parent());
		//qDebug() << itemCWidget(item)->full_name;
	}
	if (column == 3) e->setComment(item->text(column));
	c_name = item->text(0);
	c_comment = item->text(3);
	emit changed();
}


void QPIConfigWidget::typeChange(int t, UComboBox * c) {
	ConfigValueWidget * cw = (ConfigValueWidget * )c->property("qpic_widget").toLongLong();
	cw->setType(types.key(s_types[t]));
	conf->getValue(cw->full_name).setType(types.key(s_types[t]));
	emit changed();
}


void QPIConfigWidget::valueChange(ConfigValueWidget * w, QString v) {
	conf->getValue(w->full_name).setValue(v);
	emit changed();
}


void QPIConfigWidget::on_actionAddItem_triggered() {
	if (conf == 0 || !active) return;
	QString fp;
	if (pi == 0) pi = invisibleRootItem();
	else fp = itemCWidget(pi)->full_name + conf->delim;
	new_dialog.reset();
	if (new_dialog.exec() == QDialog::Rejected) return;
	QPIConfig::Entry * e;
	if (pi->childCount() == 0) {
		//qDebug() << "pi empty, remove " << itemCWidget(pi)->full_name;
		conf->removeEntry(itemCWidget(pi)->full_name, false);
	}
	//qDebug() << "add " << fp + new_dialog.name();
	e = &(conf->addEntry(fp + new_dialog.name(), new_dialog.value().isEmpty() ? "0" : new_dialog.value(), new_dialog.type(), false));
	expandItem(pi);
	pi = addEntry(pi, e);
	pi->setText(0, new_dialog.name());
	pi->setText(3, new_dialog.comment());
	int ind = s_types.indexOf(types[new_dialog.type()]);
	if (ind < 0) w_types.back()->setCurrentIndex(0);
	else w_types.back()->setCurrentIndex(ind);
	emit changed();
}


void QPIConfigWidget::on_actionAddNode_triggered() {
	if (conf == 0 || !active) return;
	QString fp;
	if (pi == 0) pi = invisibleRootItem();
	else fp = itemCWidget(pi)->full_name + conf->delim;
	new_dialog.reset(true);
	if (new_dialog.exec() == QDialog::Rejected) return;
	QPIConfig::Entry e;
	//e = &(conf->addEntry(fp + new_dialog.name(), "", "", false));
	e._full_name = fp + new_dialog.name();
	expandItem(pi);
	pi = addEntry(pi, &e, true);
	pi->setText(0, new_dialog.name());
	pi->setText(3, new_dialog.comment());
	setItemWidget(pi, 2, 0);
	emit changed();
}


void QPIConfigWidget::on_actionRemove_triggered() {
	//hide();
	if (conf == 0 || !active) return;
	QList<QTreeWidgetItem * > si = selectedItems();
	conf->buildFullNames(&(conf->root));
	QPIConfig::Entry * e;
	foreach (QTreeWidgetItem * i, si) {
		e = itemEntry(i);
		if (e == 0) continue;
		//qDebug() << "remove " + e->_full_name;
		conf->removeEntry(e->_full_name, false);
		deleteEntry(i);
	}
	emit changed();
	//show();
}


void QPIConfigWidget::clear() {
	if (!active) return;
	bool hidden = isHidden();
	hide();
	QTreeWidget::clear();
	foreach (ConfigValueWidget * i, w_values)
		delete i;
	foreach (QComboBox * i, w_types)
		delete i;
	w_values.clear();
	w_types.clear();
	if (!hidden) show();
}


void QPIConfigWidget::buildTree() {
	if (!active) return;
	if (conf == 0) return;
	bool hidden = isHidden();
	hide();
	clear();
	conf->buildFullNames(&(conf->root));
	buildEntry(invisibleRootItem(), &conf->rootEntry());
	if (!hidden) show();
}


void QPIConfigWidget::setReadOnlyValue(bool yes) {
	read_only_value = yes;
	foreach (ConfigValueWidget * i, w_values)
		i->setEnabled(!yes);
}


void QPIConfigWidget::setReadOnlyType(bool yes) {
	read_only_type = yes;
	foreach (QComboBox * i, w_types) {
		i->setEnabled(!yes);
		i->setFrame(!yes);
	}
}


void QPIConfigWidget::buildEntry(QTreeWidgetItem * i, QPIConfig::Entry * e) {
	foreach (QPIConfig::Entry * j, e->children())
		buildEntry(addEntry(i, j, !j->isLeaf()), j);
}


void QPIConfigWidget::buildFullNames(QTreeWidgetItem * i) {
	ConfigValueWidget * cw, * pw;
	cw = itemCWidget(i);
	if (i->parent() != 0) {
		pw = itemCWidget(i->parent());
		cw->full_name = pw->full_name + conf->delim + i->text(0);
	} else cw->full_name = i->text(0);
	for (int j = 0; j < i->childCount(); ++j)
		buildFullNames(i->child(j));
}


QPIConfig::Entry * QPIConfigWidget::itemEntry(QTreeWidgetItem * i) {
	ConfigValueWidget * cfw = itemCWidget(i);
	if (cfw == 0) return 0;
	return &(conf->getValue(cfw->full_name));
}


QTreeWidgetItem * QPIConfigWidget::addEntry(QTreeWidgetItem * i, QPIConfig::Entry * e, bool node) {
	if (conf == 0) return 0;
	ti = new QTreeWidgetItem();
	ti->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	ti->setSizeHint(0, QSize(26, 26));
	ti->setText(0, e->name());
	ti->setText(3, e->comment());
	w_values.push_back(new ConfigValueWidget);
	w_values.back()->setEntry(e);
	w_values.back()->setEnabled(!read_only_value);
	if (!node) {
		w_types.push_back(new UComboBox());
		w_types.back()->addItems(s_types);
		w_types.back()->setCurrentIndex(s_types.indexOf(types[e->type().leftJustified(1).left(1)]));
		w_types.back()->setProperty("qpic_widget", QVariant((qlonglong)w_values.back()));
		w_types.back()->setEnabled(!read_only_type);
		w_types.back()->setFrame(!read_only_type);
		connect(w_types.back(), SIGNAL(currentIndexChanged(int, UComboBox * )), this, SLOT(typeChange(int,UComboBox * )));
	}
	connect(w_values.back(), SIGNAL(changed(ConfigValueWidget * , QString)), this, SLOT(valueChange(ConfigValueWidget * , QString)));
	i->addChild(ti);
	setItemWidget(ti, 1, w_values.back());
	if (!node) {
		setItemWidget(ti, 2, w_types.back());
		if (itemTWidget(i) != 0) { //itemTWidget(i)->setEnabled(false);
			w_types.remove(w_types.indexOf(itemTWidget(i)));
			setItemWidget(i, 2, 0);
		}
	}
	return ti;
}


void QPIConfigWidget::deleteEntry(QTreeWidgetItem * i) {
	ConfigValueWidget * vw;
	UComboBox * cb;
	int cc = i->childCount();
	for (int j = 0; j < cc; ++j)
		deleteEntry(i->child(0));
	vw = qobject_cast<ConfigValueWidget * >(itemWidget(i, 1));
	cb = qobject_cast<UComboBox * >(itemWidget(i, 2));
	if (vw != 0) {
		w_values.remove(w_values.indexOf(vw));
		delete vw;
	}
	if (cb != 0) {
		w_types.remove(w_types.indexOf(cb));
		delete cb;
	}
	delete i;
}


bool QPIConfigWidget::filter(const QString & f, QTreeWidgetItem * i) {
	if (i->childCount() == 0) return filterItem(f, i);
	bool found = false;
	for (int j = 0; j < i->childCount(); ++j)
		if (filter(f, i->child(j))) found = true;
	i->setHidden(!found);
	return found;
}


bool QPIConfigWidget::filterItem(const QString & f, QTreeWidgetItem * i) {
	if (f.isEmpty()) {
		i->setHidden(false);
		return true;
	}
	bool ret = (!isColumnHidden(0) && i->text(0).indexOf(f, 0, Qt::CaseInsensitive) >= 0) ||
			   (!isColumnHidden(1) && itemCWidget(i)->value.indexOf(f, 0, Qt::CaseInsensitive) >= 0) ||
			   (!isColumnHidden(3) && i->text(3).indexOf(f, 0, Qt::CaseInsensitive) >= 0);
	if (itemTWidget(i) != 0)
		ret = ret || (!isColumnHidden(2) && itemTWidget(i)->currentText().indexOf(f, 0, Qt::CaseInsensitive) >= 0);
	i->setHidden(!ret);
	return ret;
}


void QPIConfigWidget::translate() {
	QStringList l;
	l << tr("Name") << tr("Value") << tr("Type") << tr("Comment");
	if (active) setHeaderLabels(l);
	types.clear();
	s_types.clear();
	addTrEntry("s", tr("string"));
	addTrEntry("l", tr("string list"));
	addTrEntry("n", tr("integer"));
	addTrEntry("f", tr("float"));
	addTrEntry("b", tr("boolean"));
	addTrEntry("c", tr("color"));
	addTrEntry("r", tr("rectangle"));
	addTrEntry("a", tr("area"));
	addTrEntry("p", tr("point"));
	addTrEntry("v", tr("vector"));
	addTrEntry("i", tr("ip"));
	actionAddItem.setText(tr("Add item ..."));
	actionAddNode.setText(tr("Add node ..."));
	actionToItem.setText(tr("Convert to item"));
	actionToNode.setText(tr("Convert to node"));
	actionRemove.setText(tr("Remove"));
	actionExpandAll.setText(tr("Expand all"));
	actionCollapseAll.setText(tr("Collapse all"));
	if (!active) return;
	for (int i = 0; i < 4; ++i)
		setColumnHidden(i, c_hidden[i]);
}


QString QPIConfigWidget::writeToString() {
	if (conf == 0) return QString();
	conf->buildFullNames(&(conf->root));
	return conf->writeAllToString();
}


void QPIConfigWidget::readFromString(QString str) {
	if (conf == 0) return;
	conf->readAllFromString(str);
	buildTree();
}
