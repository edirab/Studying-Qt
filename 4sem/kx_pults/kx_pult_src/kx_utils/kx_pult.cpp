#include <QScrollBar>
#include <QMessageBox>
#include <math.h>
#include "kx_pult.h"
#include "ui_kx_pult.h"
#include "piqt.h"
#include "qpiconfig.h"


Q_DECLARE_METATYPE(KX_X_Data)

bool isNormalDouble(const double & v) {
#ifdef WINDOWS
	return true;
#else
	return !std::isnan(v) && !std::isinf(v);
#endif
}


XCheck::XCheck(int index): QWidget() {
	index_ = index;
	setProperty("index", index);
	setMouseTracking(true);
	check.setText(QString::number(index + 1) + " ");
	check.setAutoFillBackground(true);
	spin.setMaximum(KX_X_COUNT - 1);
	QBoxLayout * l = new QBoxLayout(QBoxLayout::LeftToRight);
	l->setMargin(0);
	l->setSpacing(2);
	l->addWidget(&check);
	l->addWidget(&spin);
	setLayout(l);
	//check.installEventFilter(this);
	//spin.installEventFilter(this);
	connect(&spin, SIGNAL(valueChanged(int)), this, SLOT(spinChanged(int)));
	connect(&check, SIGNAL(toggled(bool)), this, SLOT(checkChanged_(bool)));
}


bool XCheck::eventFilter(QObject * o, QEvent * e) {
	if (e->type() == QEvent::Enter)
		qApp->postEvent(this, new QEvent(e->type()));
	return QWidget::eventFilter(o, e);
}




KX_Pult::KX_Pult(): QMainWindow(), config_("kx_pult.conf"), name_x("x"), name_c("c"),
config(piqt(config_), QIODevice::ReadWrite), coeffs(config_, "k", true) {
	//cout << sizeof(coeffsK.k_protocol->to_k) << endl;
    qRegisterMetaType<KX_X_Data>();
	ui = new Ui::KX_Pult();
	ui->setupUi(this);
	ui->configWidget->setQPIConfig(&config);
	ui->configWidget->expandAll();
	ui->list->viewport()->installEventFilter(this);
	ui->treeK->viewport()->installEventFilter(this);
	ui->scrollArea->setAutoFillBackground(false);
	ui->scrollAreaWidgetContents->setAutoFillBackground(false);
	ui->widget->setAutoFillBackground(false);
	log_menu.addAction(ui->actionClear);
	prot_x = 0;
	prot_c = 0;
	show_x = config.getValue("show_x", true);
	if (!show_x)
		ui->tabWidget->removeTab(1);
	session.setFile("session_KX_Pult.conf");
	session.addEntry(this);
	session.addEntry(ui->tabWidget);
	session.addEntry(ui->checkKHideEmpty);
	session.addEntry(ui->checkKHideNormal);
	session.addEntry(ui->checkKHideExpressions);
	session.addEntry(ui->checkKAutoCalculate);
	needWrite = isPause = false;
	timer = 0;
	//x.resize(KX_X_PACKET_NUM);
	//k.resize(K_NUM);
	QPalette pal = palette();
	QColor col;
	ui->graphic->setGraphicsCount(0);
	for (int i = 0; i < KX_X_PACKET_NUM; ++i) {
		XCheck * xc = new XCheck(i);
		xc->installEventFilter(this);
		connect(xc, SIGNAL(valueChanged(int, int)), this, SLOT(changedX(int,int)));
		connect(xc, SIGNAL(checkChanged(int, bool)), this, SLOT(toggledX(int, bool)));
		col = QColor::fromHsv(360 / KX_X_PACKET_NUM * i, 255, 200);
		pal.setColor(QPalette::Button, col);
		pal.setColor(QPalette::Window, col);
		pal.setColor(QPalette::WindowText, invertColor(col));
		xc->check.setPalette(pal);
		ui->graphic->addGraphic(QString::number(i), col);
		ui->graphic->setGraphicVisible(false, i);
		checks << xc;
		((QGridLayout * )(ui->widgetChecks->layout()))->addWidget(xc, (i / 10) * 2, i % 10);
		QLabel * lbl = new QLabel("0"); lbl->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		values << lbl;
		((QGridLayout * )(ui->widgetChecks->layout()))->addWidget(lbl, (i / 10) * 2 + 1, i % 10);
		//xc->show();
	}
	renew();
	icon_record = QIcon(":/icons/media-record.png");
	icon_stop = QIcon(":/icons/media-playback-stop.png");
	outdir = dir.absolutePath();
	if (!dir.exists()) dir.mkdir(outdir);
	outdir += "/";
	ui->treeK->setColumnWidth(0, 60);
	ui->treeK->setColumnWidth(1, 250);
	ui->treeK->setColumnWidth(3, 100);
	ui->treeK->setColumnWidth(4, 100);
	//ui->table->setK(coeffsK.k()->data(), coeffsK.count());
	ui->spinSize->setValue(K.size_s());
	addToList(trUtf8("Read K file \"%1\": %2 coeffs, %3 bytes").arg(PI2QString(coeffs.fileName())).arg(K.size_s()).arg(coeffs.k_content.size_s()), Qt::darkMagenta);
    CONNECT(void, &coeffs, sendFailed, this, pip_sendFailed);
	CONNECT(void, &coeffs, sendSucceed, this, pip_sendSucceed);
	CONNECT(void, &coeffs, receiveFailed, this, pip_receiveFailed);
	CONNECT(void, &coeffs, receiveSucceed, this, pip_receiveSucceed);
	connect(this, SIGNAL(q_k_sendFailed()), this, SLOT(k_sendFailed()), Qt::QueuedConnection);
	connect(this, SIGNAL(q_k_sendSucceed()), this, SLOT(k_sendSucceed()), Qt::QueuedConnection);
	connect(this, SIGNAL(q_k_receiveFailed()), this, SLOT(k_receiveFailed()), Qt::QueuedConnection);
	connect(this, SIGNAL(q_k_receiveSucceed()), this, SLOT(k_receiveSucceed()), Qt::QueuedConnection);
	connect(&timer_diag, SIGNAL(timeout()), this, SLOT(updateDiag()));
	connect(&session, SIGNAL(loading(QPIConfig&)), this, SLOT(loading(QPIConfig&)));
	connect(&session, SIGNAL(saving(QPIConfig&)), this, SLOT(saving(QPIConfig&)));
	connect(ui->checkKHideEmpty, SIGNAL(toggled(bool)), this, SLOT(filterTree()));
	connect(ui->checkKHideNormal, SIGNAL(toggled(bool)), this, SLOT(filterTree()));
	connect(ui->checkKHideExpressions, SIGNAL(toggled(bool)), this, SLOT(filterTree()));
	connect(ui->lineKSearch, SIGNAL(textChanged(QString)), this, SLOT(filterTree()));
	session.load();
	updateKDesc();
	updateCDesc();
    timer_diag.start(40);
	timer_update = startTimer(25);
}


KX_Pult::~KX_Pult() {
	session.save();
}


void KX_Pult::loading(QPIConfig & conf) {
	kdesc_file = conf.getValue("kdesc_file").stringValue();
	cdesc_file = conf.getValue("cdesc_file").stringValue();
}


void KX_Pult::saving(QPIConfig & conf) {
	conf.setValue("kdesc_file", kdesc_file);
	conf.setValue("cdesc_file", cdesc_file);
}


bool KX_Pult::eventFilter(QObject * o, QEvent * e) {
	if (o == ui->list->viewport()) {
		if (e->type() == QEvent::ContextMenu) {
			clear_target = 0;
			log_menu.popup(((QContextMenuEvent*)e)->globalPos());
		}
		return QMainWindow::eventFilter(o, e);
	}
	if (o == ui->treeK->viewport()) {
		if (e->type() == QEvent::ContextMenu) {
			clear_target = 1;
			log_menu.popup(((QContextMenuEvent*)e)->globalPos());
		}
		return QMainWindow::eventFilter(o, e);
	}
	int ind = o->property("index").toInt();
	//qDebug() << "event" << i << e->type();
	switch (e->type()) {
	case QEvent::Enter:
		ui->graphic->setAutoUpdate(false);
		for (int i = 0; i < KX_X_PACKET_NUM; ++i)
			ui->graphic->setGraphicLineWidth(ind == i ? 3. : 1., i);
		ui->graphic->setAutoUpdate(true);
		ui->graphic->update();
		break;
	case QEvent::Leave:
		ui->graphic->setAutoUpdate(false);
		for (int i = 0; i < KX_X_PACKET_NUM; ++i)
			ui->graphic->setGraphicLineWidth(1., i);
		ui->graphic->setAutoUpdate(true);
		ui->graphic->update();
		break;
	default: break;
	}
	return QMainWindow::eventFilter(o, e);
}


void KX_Pult::timerEvent(QTimerEvent * e) {
	if (e->timerId() == timer_update) {
		if (need_update) {
			need_update = false;
			ui->graphic->updateGraphics();
		}
	}
	if (e->timerId() == timer) {
		static QString sPI = QString::number(atan(1) * 4., 'f', 14).leftJustified(14);
		static int cnt = 0;
		int si = qMax<int>(cnt - 6, 0);
		++cnt;
		cnt %= 23;
		ui->labelWait->setText(QString(si, QChar(' ')) + sPI.mid(cnt - 6, 6).trimmed());
		if (!coeffs.isReady()) return;
		//ui->table->setK(coeffsK.k()->data(), coeffsK.count());
		//ui->table->showK();
	}
}


void KX_Pult::setControlsEnable(bool enable) {
	foreach (XCheck * i, checks)
		i->setEnabled(enable);
	ui->buttonShowAll->setEnabled(enable);
	ui->buttonHideAll->setEnabled(enable);
}


void KX_Pult::setX(const KX_X_Data & data) {
    //return;
	if (!show_x) return;
	for (int i = 0; i < KX_X_PACKET_NUM; ++i) {
		if (!isNormalDouble(data.x_data[i])) continue;
		ui->graphic->addPoint(data.x_data[i], i, false);
		values[i]->setText(QString("(%1): %2").arg(data.x_num[i]).arg(data.x_data[i]));
	}
	if (!isPause) {
		need_update = true;
	}
	if (!needWrite) return;
	stream << QString::number(tm.elapsed() / 1000., 'f', 3) << " " << QTime::currentTime().toString("hh:mm:ss") << " " << wcnt++;
	for (int i = 0; i < KX_X_PACKET_NUM; ++i)
		stream << " " << QString::number(data.x_data[i], 'f', 4);
	stream << "\n";
}


void KX_Pult::addToList(const QString & s, const QColor & c) {
	ui->list->addItem(QDateTime::currentDateTime().toString("dd/MM/yyyy  hh:ss  -  ") + s);
	ui->list->item(ui->list->count() - 1)->setTextColor(c);
	ui->list->scrollToBottom();
}


void KX_Pult::on_buttonRecord_clicked() {
	static bool isRec = false;
	int cinc = 0;
	QString str;
	isRec = !isRec;
	setControlsEnable(!isRec);
	if (isRec) {
		tm.restart();
		file.close();
		file.setFileName(outdir + getNewFileName(cinc));
		while (file.exists())
			file.setFileName(outdir + getNewFileName(cinc++));
		file.open(QIODevice::ReadWrite);
		stream.setDevice(&file);
		stream << "T V C";
		for (int i = 0; i < KX_X_PACKET_NUM; ++i)
			stream << " X" + QString::number(checks[i]->spin.value());
		stream << "\n";
		wcnt = 0;
		needWrite = true;
		ui->buttonRecord->setText(trUtf8("Finish record"));
		ui->buttonRecord->setIcon(icon_stop);
		emit recordStarted(QFileInfo(file).completeBaseName());
	} else {
		needWrite = false;
		stream.setDevice(0);
		file.close();
		ui->buttonRecord->setText(trUtf8("Start record"));
		ui->buttonRecord->setIcon(icon_record);
		emit recordStopped(QFileInfo(file).completeBaseName());
	}
}


void KX_Pult::on_actionClear_triggered() {
	switch (clear_target) {
	case 0:
		ui->list->clear();
		break;
	case 1:
		clearSelected();
		break;
	default: break;
	}
}


void KX_Pult::clearSelected() {
	QList<QTreeWidgetItem * > si = ui->treeK->selectedItems();
	ui->treeK->setUpdatesEnabled(false);
	ui->treeK->blockSignals(true);
	foreach (QTreeWidgetItem * i, si) {
		int ki = i->text(0).toInt();
		i->setText(2, "");
		coeffs.setFormula(ki, "");
	}
	ui->treeK->blockSignals(false);
	ui->treeK->setUpdatesEnabled(true);
	if (ui->checkKAutoCalculate->isChecked()) {
		QApplication::processEvents();
		calculate();
	}
}


QString KX_Pult::typeName(const QString & n) const {
	if (n.isEmpty()) return "";
	switch (n[0].toLatin1()) {
	case 'l': return trUtf8("list"); break;
	case 'b': return trUtf8("bool"); break;
	case 'n': return trUtf8("int"); break;
	case 'f': return trUtf8("double"); break;
	case 'c': return trUtf8("color"); break;
	case 'r': return trUtf8("rect"); break;
	case 'a': return trUtf8("rect"); break;
	case 'p': return trUtf8("point"); break;
	case 'v': return trUtf8("vector"); break;
	case 'i': return trUtf8("IP"); break;
	case 'e': return trUtf8("enum"); break;
	case 'F': return trUtf8("file"); break;
	case 'D': return trUtf8("dir"); break;
	}
	return "";
}


void KX_Pult::received(bool ok) {
	if (!ok) return;
    QMetaObject::invokeMethod(this, "setX", Qt::QueuedConnection, Q_ARG(KX_X_Data, prot_x->from_x));
}


void KX_Pult::on_treeK_itemClicked(QTreeWidgetItem * item, int column) {
	Qt::ItemFlags f = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if (column == 2) f |= Qt::ItemIsEditable;
	item->setFlags(f);
}


void KX_Pult::on_treeK_itemChanged(QTreeWidgetItem * item, int column) {
	if (column != 2) return;
	int ki = item->text(0).toInt();
	coeffs.setFormula(ki, piqt(item->text(column)));
	if (ui->checkKAutoCalculate->isChecked())
		calculate();
}


QString KX_Pult::getNewFileName(int inc) {
	dir.refresh();
	dir.setNameFilters(QStringList("Experiment_*.txt"));
	return "Experiment_" + QDateTime::currentDateTime().toString("dd_MM_yy__hh_mm_ss") + "__" +
			QString::number(dir.entryList().count() + inc) + ".txt";
}


void KX_Pult::on_buttonSendK_clicked() {
	on_buttonWrite_clicked();
	coeffs.sendCoeffs();
	if (timer != 0) killTimer(timer);
	timer = startTimer(100);
}


void KX_Pult::on_buttonReceiveK_clicked() {
	coeffs.receiveCoeffs();
	if (timer != 0) killTimer(timer);
	timer = startTimer(100);
}


void KX_Pult::on_buttonShowAll_clicked() {
	for (int i = 0; i < KX_X_PACKET_NUM; ++i) {
		checks[i]->check.setChecked(true);
		//ui->graphic->setGraphicVisible(true, i);
	}
}


void KX_Pult::on_buttonHideAll_clicked() {
	for (int i = 0; i < KX_X_PACKET_NUM; ++i) {
		checks[i]->check.setChecked(false);
		//ui->graphic->setGraphicVisible(false, i);
	}
}


void KX_Pult::on_buttonRead_clicked() {
	coeffs.readCoeffs();
	addToList(trUtf8("Read K file \"%1\": %2 coeffs, %3 bytes").arg(PI2QString(coeffs.fileName())).arg(K.size_s()).arg(coeffs.k_content.size_s()), Qt::darkMagenta);
	updateTree();
}


void KX_Pult::on_buttonWrite_clicked() {
	coeffs.writeCoeffs();
	addToList(trUtf8("Write K file \"%1\": %2 coeffs, %3 bytes").arg(PI2QString(coeffs.fileName())).arg(K.size_s()).arg(coeffs.k_content.size_s()), Qt::darkMagenta);
}


void KX_Pult::on_buttonResize_clicked() {
	K.resize(ui->spinSize->value());
	coeffs.formulas.resize(ui->spinSize->value());
	ui->spinSize->setStyleSheet("");
	updateTree();
}


void KX_Pult::on_buttonSetKDesc_clicked() {
	QString ret = QFileDialog::getOpenFileName(this, trUtf8("Select *.h file with K description"), kdesc_file, "C/C++ header files(*.h *.hpp);;All files(*)");
	if (ret.isEmpty()) return;
	kdesc_file = QDir::current().relativeFilePath(ret);
	updateKDesc(true);
}


void KX_Pult::on_buttonSetCDesc_clicked() {
	QString ret = QFileDialog::getOpenFileName(this, trUtf8("Select *.h file with C description"), cdesc_file, "C/C++ header files(*.h *.hpp);;All files(*)");
	if (ret.isEmpty()) return;
	cdesc_file = QDir::current().relativeFilePath(ret);
	updateCDesc();
}


void KX_Pult::on_spinSize_valueChanged(int) {
	ui->spinSize->setStyleSheet("");
}


void KX_Pult::k_sendFailed() {
	stopWait();
	addToList(trUtf8("K not sended"), Qt::darkRed);
}


void KX_Pult::k_sendSucceed() {
	stopWait();
	addToList(trUtf8("K sended"), Qt::darkGreen);
}


void KX_Pult::k_receiveFailed() {
	stopWait();
	addToList(trUtf8("K not received"), Qt::darkRed);
}


void KX_Pult::k_receiveSucceed() {
	stopWait();
	addToList(trUtf8("K received"), Qt::darkGreen);
	addToList(trUtf8("Write K file \"%1\": %2 coeffs, %3 bytes").arg(PI2QString(coeffs.fileName())).arg(K.size_s()).arg(coeffs.k_content.size_s()), Qt::darkMagenta);
	ui->spinSize->setValue(K.size_s());
	updateTree();
	//ui->table->setK(coeffsK.k()->data(), coeffsK.count());
}


void KX_Pult::on_spinBuffer_editingFinished() {
	ui->graphic->setHistorySize(ui->spinBuffer->value());
}


void KX_Pult::stopWait() {
	killTimer(timer);
	timer = 0;
	ui->labelWait->setText(" ");
}


void KX_Pult::updateGraph() {
	ui->graphic->updateGraphics();
}


void KX_Pult::updateDiag() {
	ui->labelKReceiver->setText(piqt(coeffs.k_protocol->receiverDeviceName() + " - " + coeffs.k_protocol->receiverDeviceState()));
	ui->labelKSender->setText(piqt(coeffs.k_protocol->senderDeviceName()));
	ui->spinKSended->setValue(coeffs.k_protocol->sendCount());
	ui->spinKReceived->setValue(coeffs.k_protocol->receiveCount());
	ui->spinKWrong->setValue(coeffs.k_protocol->wrongCount());
	ui->spinKMissed->setValue(coeffs.k_protocol->missedCount());
	ui->labelKType->setText("0x" + QString::number(coeffs.k_protocol->from_k.type, 16).toUpper().rightJustified(2, '0'));
	ui->labelKAddrPult->setText("0x" + QString::number(coeffs.k_protocol->from_k.addr_to, 16).toUpper().rightJustified(2, '0'));
	ui->labelKAddr->setText("0x" + QString::number(coeffs.k_protocol->to_k.addr_to, 16).toUpper().rightJustified(2, '0'));
	
	ui->labelXReceiver->setText(piqt(prot_x->receiverDeviceName() + " - " + prot_x->receiverDeviceState()));
	ui->labelXSender->setText(piqt(prot_x->senderDeviceName()));
	ui->spinXSended->setValue(prot_x->sendCount());
	ui->spinXReceived->setValue(prot_x->receiveCount());
	ui->spinXWrong->setValue(prot_x->wrongCount());
	ui->spinXMissed->setValue(prot_x->missedCount());
	ui->labelXType->setText("0x" + QString::number(prot_x->from_x.type, 16).toUpper().rightJustified(2, '0'));
	ui->labelXAddrPult->setText("0x" + QString::number(prot_x->from_x.addr_to, 16).toUpper().rightJustified(2, '0'));
	ui->labelXAddr->setText("0x" + QString::number(prot_x->to_x.addr_to, 16).toUpper().rightJustified(2, '0'));

	ui->labelCReceiver->setText(piqt(prot_c->receiverDeviceName() + " - " + prot_c->receiverDeviceState()));
	ui->labelCSender->setText(piqt(prot_c->senderDeviceName()));
	ui->spinCSended->setValue(prot_c->sendCount());
	ui->spinCReceived->setValue(prot_c->receiveCount());
	ui->spinCWrong->setValue(prot_c->wrongCount());
	ui->spinCMissed->setValue(prot_c->missedCount());
	ui->labelCType->setText("0x" + QString::number(prot_c->from_c.type, 16).toUpper().rightJustified(2, '0'));
	ui->labelCAddrPult->setText("0x" + QString::number(prot_c->from_c.addr_to, 16).toUpper().rightJustified(2, '0'));
	ui->labelCAddr->setText("0x" + QString::number(prot_c->to_c.addr_to, 16).toUpper().rightJustified(2, '0'));
}


int KX_Pult::parseHeader(const QString & file, QMap<int, KX_Pult::KDesc> & map) {
	map.clear();
	QFile f(file);
	if (!f.open(QIODevice::ReadOnly)) {
		updateTree();
		addToList(trUtf8("Update descriptions from \"%1\": error").arg(file), Qt::darkRed);
		return 0;
	}
	addToList(trUtf8("Update descriptions from \"%1\"").arg(file), Qt::darkMagenta);
	QTextStream s(&f);
	int cind = -1;
	bool found = false;
	//qDebug() << "\nparse" << file;
	while (!s.atEnd()) {
		QString line = s.readLine().trimmed(), num, name, type, comment;
		int i = line.indexOf("//");
		if (i >= 0) {
			comment = line.right(line.length() - i - 2);
			type = comment.left(1);
			comment = comment.right(comment.length() - 1).trimmed();
			line = line.left(i).trimmed();
		}
		if (line.isEmpty()) continue;
		if (line.contains("enum")) {
			found = true;
			continue;
		}
		if (!found) continue;
		if (line.contains('}'))
			break;
		line.remove(',').remove(' ').remove('\t');
		i = line.indexOf("=");
		if (i >= 0) {
			num = line.right(line.length() - i - 1).trimmed();
			line = line.left(i).trimmed();
		}
		name = line;
		if (num.isEmpty())
			++cind;
		else
			cind = Q2PIString(num).toInt();
		KDesc kd;
		kd.index = cind;
		kd.name = name;
		kd.type = type;
		kd.comment = comment;
		map[kd.index] = kd;
		//qDebug() << name << cind << type << comment;
	}
	cind++;
	return cind;
}


void KX_Pult::updateKDesc(bool ask_move) {
	int cind = parseHeader(kdesc_file, kdesc);
	if (K.size_s() < cind) {
		ui->spinSize->setValue(cind);
		ui->spinSize->setStyleSheet("background-color: rgb(220, 220, 255);");
	}
	bool move = false;
	if (ask_move)
		move = (QMessageBox::question(this, "KX Pult", "Save values at associated names?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes);
	updateTree(move);
}


void KX_Pult::updateCDesc() {
	parseHeader(cdesc_file, cdesc);
	updateCommands();
}


bool stringComp(const QString & s1, const QString & s2) {
	if (s1.size() != s2.size())
		return s1.size() > s2.size();
	return s1 > s2;
}


void KX_Pult::updateTree(bool move) {
	int sp = ui->treeK->verticalScrollBar()->value();
	QApplication::setOverrideCursor(Qt::WaitCursor);
	//qDebug() << "fill tree ...";
	QMap<QString, QString> prev_val;
	if (move) {
		for (int i = 0; i < ui->treeK->topLevelItemCount(); ++i) {
			QTreeWidgetItem * ti = ui->treeK->topLevelItem(i);
			if (!ti->text(1).isEmpty())
				prev_val[ti->text(1)] = ti->text(2);
		}
	}
	ui->treeK->clear();
	ui->treeK->setUpdatesEnabled(false);
	eval.clearCustomVariables();
	for (int i = 0; i < K.size_s(); ++i) {
		QTreeWidgetItem * ti = new QTreeWidgetItem();
		KDesc kd = kdesc[i];
		QString kn = QString("k%1").arg(i);
		knames[kn] = i;
		knames_sort << kn;
		if (eval.content.findVariable(kn) < 0)
			eval.content.addVariable(kn, 0., false);
		if (!kd.name.isEmpty()) {
			knames[kd.name] = i;
			knames_sort << kd.name;
			eval.content.addVariable(kd.name, 0., false);
		}
		if (move && !kd.name.isEmpty()) {
			if (prev_val.contains(kd.name))
				coeffs.setFormula(i, Q2PIString(prev_val[kd.name]));
		}
		ti->setText(0, QString::number(i));
		ti->setText(1, kd.name);
		ti->setText(2, PI2QString(coeffs.formula(i)));
		ti->setText(3, QString::number(K[i]));
		ti->setText(4, typeName(kd.type));
		ti->setText(5, kd.comment);
		ui->treeK->addTopLevelItem(ti);
	}
	eval.content.sortVariables();
	//qDebug() << "fill tree ok";
	//qDebug() << "sort ...";
	qSort(knames_sort.begin(), knames_sort.end(), stringComp);
	//qDebug() << "names" << knames_sort;
	//qDebug() << "sort ok";
	QApplication::restoreOverrideCursor();
	ui->treeK->setUpdatesEnabled(true);
	ui->treeK->verticalScrollBar()->setValue(sp);
	calculate();
	filterTree();
}


void KX_Pult::updateCommands() {
	while (ui->layoutCommands->count() > 0)
		delete ui->layoutCommands->itemAt(0)->widget();
	QMapIterator<int, KDesc> it(cdesc);
	while (it.hasNext()) {
		it.next();
		KDesc kd = it.value();
		QPushButton * b = new QPushButton();
		QString text = kd.name;
		if (!kd.comment.isEmpty())
			text += QString("\n(%1)").arg(kd.comment);
		b->setText(text);
		b->setProperty("_command", kd.index);
		connect(b, SIGNAL(clicked()), this, SLOT(commandClicked()));
		ui->layoutCommands->addWidget(b);
	}
}


void KX_Pult::filterTree() {
	bool he = ui->checkKHideEmpty->isChecked();
	bool hn = ui->checkKHideNormal->isChecked();
	bool hs = ui->checkKHideExpressions->isChecked();
	bool ok = false;
	QString fl = ui->lineKSearch->text();
	int lc = ui->treeK->topLevelItemCount();
	for (int i = 0; i < lc; ++i) {
		QTreeWidgetItem * ti = ui->treeK->topLevelItem(i);
		if (ti->text(1).isEmpty() && he)
			ti->setHidden(true);
		else
			if (fl.isEmpty())
				ti->setHidden(false);
			else
				ti->setHidden(!ti->text(0).contains(fl, Qt::CaseInsensitive) &&
							  !ti->text(1).contains(fl, Qt::CaseInsensitive) &&
							  !ti->text(2).contains(fl, Qt::CaseInsensitive) &&
							  !ti->text(3).contains(fl, Qt::CaseInsensitive) &&
							  !ti->text(4).contains(fl, Qt::CaseInsensitive));
		if (hn)
			if (ti->data(0, Qt::UserRole).toBool())
				ti->setHidden(true);
		if (hs) {
			ti->data(2, Qt::DisplayRole).toDouble(&ok);
			if (!ok)
				ti->setHidden(true);
		}
	}
}


void KX_Pult::calculate() {
	calculated.clear();
	ui->treeK->setUpdatesEnabled(false);
	ui->treeK->blockSignals(true);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	progress(0, 100);
	ui->buttonCalculate->setEnabled(false);
	QApplication::processEvents();
	for (int i = 0; i < K.size_s(); ++i) {
		ui->treeK->topLevelItem(i)->setToolTip(2, QString());
		ui->treeK->topLevelItem(i)->setToolTip(3, QString());
	}
	for (int i = 0; i < K.size_s(); ++i) {
		progress(i, K.size_s());
		calculateExpression(i, QVector<int>());
	}
	ui->buttonCalculate->setEnabled(true);
	ui->progress->setValue(100);
	QApplication::restoreOverrideCursor();
	ui->treeK->blockSignals(false);
	ui->treeK->setUpdatesEnabled(true);
}


bool KX_Pult::calculateExpression(int i, QVector<int> trace) {
	if (calculated.contains(i)) return true;
	trace << i;
	QTreeWidgetItem * ti = ui->treeK->topLevelItem(i);
	QString expr = ti->text(2);
	if (expr.isEmpty() || expr == "0" || expr == "0,00000000" || expr == "0.00000000") {
		markNormal(ti);
		calculated << i;
		K[i] = 0.;
		ti->setText(3, "0");
		return true;
	}
	//ti->setToolTip(2, QString());
	if (!eval.check(expr)) {
		markError(ti, eval.error());
		return false;
	}
	foreach (const QString & n, knames_sort) {
		if (expr.contains(n)) {
			int ki = knames.value(n, -1);
			if (trace.contains(ki)) {
				QString strace;
				trace << ki;
				for (int j = 0; j < trace.size(); ++j) {
					//calculated << trace[j];
					if (j > 0) strace += " -> ";
					strace += "k" + QString::number(trace[j]);
				}
				for (int j = 0; j < trace.size(); ++j) {
					QTreeWidgetItem * pti = ui->treeK->topLevelItem(trace[j]);
					markError(pti, QString("Circular dependency: %1!").arg(strace));
				}
				return false;
			}
			if (ki < 0) {
				markError(ti);
				return false;
			}
			if (calculated.contains(ki)) {
				eval.setVariable(n, K[ki]);
			} else {
				if (calculateExpression(ki, trace))
					eval.setVariable(n, K[ki]);
				else {
					markError(ti);
					return false;
				}
			}
		}
	}
	markNormal(ti);
	calculated << i;
	complexd ret = eval.evaluate();
	K[i] = ret.real();
	ti->setText(3, QString::number(K[i]));
	return true;
}


void KX_Pult::markError(QTreeWidgetItem * item, const QString & tool_tip) {
	int cc = item->columnCount();
	for (int i = 0; i < cc; ++i)
		item->setBackgroundColor(i, QColor(255, 200, 200));
	if (item->toolTip(2).isEmpty())
		item->setToolTip(2, tool_tip);
	if (item->toolTip(3).isEmpty())
		item->setToolTip(3, tool_tip);
	item->setData(0, Qt::UserRole, false);
	item->setText(3, "Error");
}


void KX_Pult::markNormal(QTreeWidgetItem * item) {
	int cc = item->columnCount();
	for (int i = 0; i < cc; ++i)
		item->setBackground(i, Qt::NoBrush);
	item->setToolTip(2, QString());
	item->setToolTip(3, QString());
	item->setData(0, Qt::UserRole, true);
}


void KX_Pult::progress(int val, int max) {
	if (ctm.elapsed() < 50) return;
	ctm.restart();
	ui->progress->setValue(qRound(val * 100. / max));
	QApplication::processEvents();
}


void KX_Pult::renew(bool write) {
	addToList(trUtf8("Update settings from \"%1\"").arg(PI2QString(config_)), Qt::darkMagenta);
	dir.setPath(config.getValue("x.output_dir", "./Experiments/").stringValue());
	setWindowTitle(config.getValue("title", "Noname").stringValue() + trUtf8(" - KX Pult"));
	//if (write) ui->configWidget->write();
	if (prot_x != 0) {
		prot_x->stop();
		delete prot_x;
	}
	if (prot_c != 0) {
		prot_c->stop();
		delete prot_c;
	}
	prot_x = new __KX_Protocol_X(config_, name_x);
	prot_c = new __KX_Protocol_C(config_, name_c);
	ui->graphic->setAutoXIncrement(prot_x->expectedFrequency() > 0. ? 1. / prot_x->expectedFrequency() : 1.);
	coeffs.renew();
	CONNECT1(void, bool, prot_x, received, this, received);
}


void KX_Pult::toggledX(int index, bool on) {
	ui->graphic->setGraphicVisible(on, index);
}


void KX_Pult::changedX(int index, int num) {
	prot_x->to_x.x_num[index] = num;
}


void KX_Pult::commandClicked() {
	QPushButton * b = qobject_cast<QPushButton*>(sender());
	if (!b) return;
	prot_c->sendCommand(b->property("_command").toInt());
}
