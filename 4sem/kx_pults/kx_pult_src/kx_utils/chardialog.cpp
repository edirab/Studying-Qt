#include "chardialog.h"
#include "ui_chardialog.h"


CharDialog::CharDialog(QWidget * parent): QDialog(parent) {
	ui = new Ui::CharDialog();
	ui->setupUi(this);
	QChar c;
	int k;
	for (int i = 0; i < 256; ++i) {
		for (int j = 0; j < 256; ++j) {
			c = QChar(j, i);
			k = c.category();
			if (chars.size() <= k)
				chars.resize(k + 1);
			if (!c.isPrint()) continue;
			
			chars[k].push_back(c);
		}
	}
	size = 30;
	QStringList cat;
	cat << tr("No Category") << tr("Mark NonSpacing") << tr("Mark SpacingCombining") << tr("Mark Enclosing")
		<< tr("Number DecimalDigit") << tr("Number Letter") << tr("Number Other") << tr("Separator Space")
		<< tr("Separator Line") << tr("Separator Paragraph") << tr("Other Control") << tr("Other Format")
		<< tr("Other Surrogate") << tr("Other PrivateUse") << tr("Other NotAssigned") << tr("Letter Uppercase")
		<< tr("Letter Lowercase") << tr("Letter Titlecase") << tr("Letter Modifier") << tr("Letter Other")
		<< tr("Punctuation Connector") << tr("Punctuation Dash") << tr("Punctuation Open") << tr("Punctuation Close")
		<< tr("Punctuation InitialQuote") << tr("Punctuation FinalQuote") << tr("Punctuation Other") << tr("Symbol Math")
		<< tr("Symbol Currency") << tr("Symbol Modifier") << tr("Symbol Other");
	ui->comboCategory->addItems(cat);
	ui->spinSize->setValue(size);
	ui->comboCategory->setCurrentIndex(27);
	ui->tableChars->viewport()->installEventFilter(this);
}


CharDialog::~CharDialog() {
	delete ui;
}


void CharDialog::setCharFont(const QFont & f) {
	QFont fnt = ui->tableChars->font();
	fnt.setFamily(f.family());
	ui->tableChars->setFont(fnt);
}


void CharDialog::changeEvent(QEvent * e) {
	if (e->type() == QEvent::LanguageChange) {
		ui->retranslateUi(this);
		QStringList cat;
		cat << tr("No Category") << tr("Mark NonSpacing") << tr("Mark SpacingCombining") << tr("Mark Enclosing")
			<< tr("Number DecimalDigit") << tr("Number Letter") << tr("Number Other") << tr("Separator Space")
			<< tr("Separator Line") << tr("Separator Paragraph") << tr("Other Control") << tr("Other Format")
			<< tr("Other Surrogate") << tr("Other PrivateUse") << tr("Other NotAssigned") << tr("Letter Uppercase")
			<< tr("Letter Lowercase") << tr("Letter Titlecase") << tr("Letter Modifier") << tr("Letter Other")
			<< tr("Punctuation Connector") << tr("Punctuation Dash") << tr("Punctuation Open") << tr("Punctuation Close")
			<< tr("Punctuation InitialQuote") << tr("Punctuation FinalQuote") << tr("Punctuation Other") << tr("Symbol Math")
			<< tr("Symbol Currency") << tr("Symbol Modifier") << tr("Symbol Other");
		int pi = ui->comboCategory->currentIndex();
		ui->comboCategory->clear();
		ui->comboCategory->addItems(cat);
		ui->comboCategory->setCurrentIndex(pi);
		return;
	}
	QDialog::changeEvent(e);
}


bool CharDialog::eventFilter(QObject * o, QEvent * e) {
	if (o == ui->tableChars->viewport()) {
		if (e->type() != QEvent::Wheel)
			return QDialog::eventFilter(o, e);
		qApp->sendEvent(ui->verticalScroll, e);
	}
	return QDialog::eventFilter(o, e);
}


void CharDialog::resizeEvent(QResizeEvent * ) {
	int r = ui->tableChars->contentsRect().height() / csize, c = ui->tableChars->contentsRect().width() / csize;
	ui->tableChars->setRowCount(r);
	ui->tableChars->setColumnCount(c);
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			if (ui->tableChars->item(i, j) == 0) {
				ui->tableChars->setItem(i, j, new QTableWidgetItem());
				ui->tableChars->item(i, j)->setTextAlignment(Qt::AlignCenter);
			}
		}
	}
	on_comboCategory_currentIndexChanged(ui->comboCategory->currentIndex());
}


void CharDialog::clear() {
	for (int i = 0; i < ui->tableChars->rowCount(); ++i)
		for (int j = 0; j < ui->tableChars->columnCount(); ++j)
			ui->tableChars->item(i, j)->setText(QString());
}


void CharDialog::on_comboCategory_currentIndexChanged(int index) {
	if (index < 0) return;
	int r = ui->tableChars->rowCount(), c = ui->tableChars->columnCount(), m;
	cur = &chars[index];
	if (r == 0) return;
	m = (cur->size() - 2) / c - r + 1;
	if (m < 0) m = 0;
	ui->verticalScroll->setMinimum(0);
	ui->verticalScroll->setMaximum(m);
	on_verticalScroll_valueChanged(ui->verticalScroll->value());
}


void CharDialog::on_verticalScroll_valueChanged(int index) {
	int ci = ui->tableChars->columnCount() * index;
	for (int i = 0; i < ui->tableChars->rowCount(); ++i) {
		for (int j = 0; j < ui->tableChars->columnCount(); ++j) {
			++ci;
			if (cur->size() > ci) ui->tableChars->item(i, j)->setText(cur->at(ci));
			else ui->tableChars->item(i, j)->setText(QString());
		}
	}
}


void CharDialog::on_spinSize_valueChanged(int index) {
	size = index;
	csize = size * 2;
	ui->tableChars->horizontalHeader()->setDefaultSectionSize(csize);
	ui->tableChars->verticalHeader()->setDefaultSectionSize(csize);
	on_comboCategory_currentIndexChanged(ui->comboCategory->currentIndex());
	QFont font = ui->tableChars->font();
	font.setPointSize(size);
	ui->tableChars->setFont(font);
	resizeEvent(0);
}


void CharDialog::on_tableChars_cellPressed(int row, int column) {
	sel_char = ui->tableChars->item(row, column)->text()[0];
}


void CharDialog::on_buttonBox_accepted() {
	emit charSelected(sel_char);
	accept();
}
