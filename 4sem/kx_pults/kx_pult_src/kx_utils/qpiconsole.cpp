#include "qpiconsole.h"


QPIConsole::QPIConsole(QWidget * parent): QTabWidget(parent) {
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
	cur_tab = timer = -1;
	def_align = Qt::AlignCenter;
}


void QPIConsole::timerEvent(QTimerEvent * ) {
	for (int i = 0; i < columns().size(); ++i) {
		Column & ccol(tabs[cur_tab].columns[i]);
		QVector<Variable> & cvars(ccol.variables);
		if (ccol.alignment.testFlag(Qt::AlignLeft)) {
			ccol.layout->setLabelAlignment(Qt::AlignLeft);
		} else {
			ccol.layout->setLabelAlignment(Qt::AlignRight);
		}
		for (int j = 0; j < cvars.size(); ++j) {
			Variable & tv(cvars[j]);
			tv.label->setText(tv.name);
			if (tv.type >= 0) {
				if (ccol.alignment.testFlag(Qt::AlignLeft)) {
					tv.label->setAlignment(Qt::AlignLeft);
					if (tv.widget != 0) tv.widget->setAlignment(Qt::AlignLeft);
				} else {
					if (ccol.alignment.testFlag(Qt::AlignRight)) {
						tv.label->setAlignment(Qt::AlignRight);
						if (tv.widget != 0) tv.widget->setAlignment(Qt::AlignRight);
					} else {
						tv.label->setAlignment(Qt::AlignRight);
						if (tv.widget != 0) tv.widget->setAlignment(Qt::AlignLeft);
					}
				}
			}
			if (tv.widget == 0) continue;
			if (tv.type <= 0 && tv.ptr == 0) continue;
			if (tv.ptr != 0) {
				switch (tv.type) {
				case 0: tv.widget->setText((tv.ptr != 0 ? *(const QString*)tv.ptr : QString())); break;
				case 1: tv.widget->setText((tv.ptr != 0 ? *(const bool*)tv.ptr : false) ? "true" : "false"); break;
				case 2: tv.widget->setText(numIntString<int>(tv.ptr != 0 ? *(const int*)tv.ptr : 0, tv.format)); break;
				case 3: tv.widget->setText(numIntString<long>(tv.ptr != 0 ? *(const long*)tv.ptr : 0l, tv.format)); break;
				case 4: tv.widget->setText(QString(tv.ptr != 0 ? *(const char*)tv.ptr : char(0))); break;
				case 5: tv.widget->setText(numFloatString<float>(tv.ptr != 0 ? *(const float*)tv.ptr : 0.f, tv.format)); break;
				case 6: tv.widget->setText(numFloatString<double>(tv.ptr != 0 ? *(const double*)tv.ptr : 0., tv.format)); break;
				case 7: tv.widget->setText(numFloatString<short>(tv.ptr != 0 ? *(const short*)tv.ptr : short(0), tv.format)); break;
				case 8: tv.widget->setText(numIntString<uint>(tv.ptr != 0 ? *(const uint*)tv.ptr : 0u, tv.format)); break;
				case 9: tv.widget->setText(numIntString<ulong>(tv.ptr != 0 ? *(const ulong*)tv.ptr : 0ul, tv.format)); break;
				case 10: tv.widget->setText(numIntString<ushort>(tv.ptr != 0 ? *(const ushort*)tv.ptr : ushort(0), tv.format)); break;
				case 11: tv.widget->setText(numIntString<uchar>(tv.ptr != 0 ? *(const uchar*)tv.ptr : uchar(0), tv.format)); break;
				case 12: tv.widget->setText(numIntString<llong>(tv.ptr != 0 ? *(const llong*)tv.ptr : 0l, tv.format)); break;
				case 13: tv.widget->setText(numIntString<ullong>(tv.ptr != 0 ? *(const ullong*)tv.ptr: 0ull, tv.format)); break;
				case 14: tv.widget->setText(numIntString<int>(bitsValue(tv.ptr, tv.bitFrom, tv.bitCount), tv.format, tv.bitCount/8)); break;
				default: break;
				}
			}
		}
	}
}


#define ADD_VAR_BODY tv.name = name; if (!tv.name.isEmpty()) tv.name += ":"; tv.bitFrom = tv.bitCount = 0; tv.format = format; checkColumn(col);
#define ADD_VAR_SBODY tv.name = name; tv.bitFrom = tv.bitCount = 0; tv.format = format; checkColumn(col);

#define ADD_VAR_QT QLabel * lbl = new QLabel(name); QLabel * w = new QLabel(); \
					lbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred); \
					w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred); \
					column(col).variables.back().label = lbl; column(col).variables.back().widget = w; \
					lbl->setAlignment(column(col).alignment); \
					applyFormat(lbl, format); applyFormat(w, format); \
					column(col).layout->addRow(lbl, w);

void QPIConsole::addString(const QString & name, int col, Formats format) {
	ADD_VAR_SBODY tv.type = -1; tv.ptr = 0; column(col).push_back(tv);
	QLabel * lbl = new QLabel(name);
	lbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	column(col).variables.back().label = lbl; column(col).variables.back().widget = 0;
	lbl->setAlignment(column(col).alignment);
	applyFormat(lbl, format);
	column(col).layout->addRow(lbl);
}
void QPIConsole::addVariable(const QString & name, const QString* ptr, int col, Formats format) {
	ADD_VAR_BODY tv.type = 0; tv.ptr = ptr; column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addVariable(const QString & name, const bool * ptr, int col, Formats format) {
	ADD_VAR_BODY tv.type = 1; tv.ptr = ptr; column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addVariable(const QString & name, const int * ptr, int col, Formats format) {
	ADD_VAR_BODY tv.type = 2; tv.ptr = ptr; column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addVariable(const QString & name, const long * ptr, int col, Formats format) {
	ADD_VAR_BODY tv.type = 3; tv.ptr = ptr; column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addVariable(const QString & name, const char * ptr, int col, Formats format) {
	ADD_VAR_BODY tv.type = 4; tv.ptr = ptr; column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addVariable(const QString & name, const float * ptr, int col, Formats format) {
	ADD_VAR_BODY tv.type = 5; tv.ptr = ptr; column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addVariable(const QString & name, const double * ptr, int col, Formats format) {
	ADD_VAR_BODY tv.type = 6; tv.ptr = ptr; column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addVariable(const QString & name, const short * ptr, int col, Formats format) {
	ADD_VAR_BODY tv.type = 7; tv.ptr = ptr; column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addVariable(const QString & name, const uint * ptr, int col, Formats format) {
	ADD_VAR_BODY tv.type = 8; tv.ptr = ptr; column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addVariable(const QString & name, const ulong * ptr, int col, Formats format) {
	ADD_VAR_BODY tv.type = 9; tv.ptr = ptr; column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addVariable(const QString & name, const ushort * ptr, int col, Formats format) {
	ADD_VAR_BODY tv.type = 10; tv.ptr = ptr; column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addVariable(const QString & name, const uchar * ptr, int col, Formats format) {
	ADD_VAR_BODY tv.type = 11; tv.ptr = ptr; column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addVariable(const QString & name, const llong * ptr, int col, Formats format) {
	ADD_VAR_BODY tv.type = 12; tv.ptr = ptr; column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addVariable(const QString & name, const ullong * ptr, int col, Formats format) {
	ADD_VAR_BODY tv.type = 13; tv.ptr = ptr; column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addBitVariable(const QString & name, const void * ptr, int fromBit, int bitCount, int col, Formats format) {
	tv.name = name; tv.bitFrom = fromBit; tv.bitCount = bitCount; tv.type = 14; tv.ptr = ptr; tv.format = format;
	checkColumn(col); column(col).push_back(tv); ADD_VAR_QT}
void QPIConsole::addEmptyLine(int col, uint count) {
	tv.name = ""; tv.type = 0; tv.ptr = 0; tv.format = Normal;
	for (uint i = 0; i < count; ++i) {
		checkColumn(col);
		column(col).push_back(tv);
		QLabel * lbl = new QLabel();
		lbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
		column(col).variables.back().label = lbl; column(col).variables.back().widget = 0;
		lbl->setAlignment(column(col).alignment);
		applyFormat(lbl, tv.format);
		column(col).layout->addRow(lbl);
	}
}


int QPIConsole::addTab(const QString & name, char bind_key) {
	QWidget * w = new QWidget();
	QVBoxLayout * lay = new QVBoxLayout();
	QHBoxLayout * clay = new QHBoxLayout();
	QLabel * lbl = new QLabel();
	lay->setContentsMargins(2, 2, 2, 2);
	clay->setContentsMargins(0, 0, 0, 0);
	lay->addLayout(clay);
	lay->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Preferred, QSizePolicy::Expanding));
	lay->addWidget(lbl);
	w->setLayout(lay);
	QTabWidget::addTab(w, name);
	tabs.push_back(Tab(name, bind_key));
	tabs.back().layout = clay;
	tabs.back().widget = lbl;
	cur_tab = tabs.size() - 1;
	setCurrentIndex(cur_tab);
	return tabs.size();
}


void QPIConsole::checkColumn(int col) {
	while (columns().size() < col) {
		QFormLayout * lay = new QFormLayout();
		lay->setContentsMargins(2, 2, 2, 2);
		lay->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
		columns().push_back(Column(def_align));
		columns().back().layout = lay;
		tabs[cur_tab].layout->addLayout(lay);
	}
}


int QPIConsole::bitsValue(const void * src, int offset, int count) const {
	int ret = 0, stbyte = offset / 8, cbit = offset - stbyte * 8;
	char cbyte = reinterpret_cast<const char * >(src)[stbyte];
	for (int i = 0; i < count; i++) {
		ret |= ((cbyte >> cbit & 1) << i);
		cbit++;
		if (cbit == 8) {
			cbit = 0;
			stbyte++;
			cbyte = reinterpret_cast<const char * >(src)[stbyte];
		}
	}
	return ret;
}


const QString & QPIConsole::toBin(const void * d, int s) {
	binstr.clear();
	uchar cc, b;
	for (int i = 0; i < s; ++i) {
		cc = ((const uchar *)d)[i];
		b = 1;
		for (int j = 0; j < 8; ++j) {
			binstr.prepend(cc & b ? "1" : "0");
			b <<= 1;
		}
		if (i < s - 1) binstr.prepend(" ");
	}
	return binstr;
}


void QPIConsole::applyFormat(QLabel * l, QPIConsole::Formats f) {
	QColor fcol = Qt::black, bcol = QColor(0xFFFFFF);
	QFont fnt = font();
	
	if (f.testFlag(QPIConsole::Bold)) fnt.setBold(true);
	if (f.testFlag(QPIConsole::Italic)) fnt.setItalic(true);
	if (f.testFlag(QPIConsole::Underline)) fnt.setUnderline(true);

	if (f.testFlag(QPIConsole::Black)) fcol = Qt::black;
	if (f.testFlag(QPIConsole::Red)) fcol = Qt::red;
	if (f.testFlag(QPIConsole::Green)) fcol = Qt::green;
	if (f.testFlag(QPIConsole::Yellow)) fcol = Qt::yellow;
	if (f.testFlag(QPIConsole::Blue)) fcol = Qt::blue;
	if (f.testFlag(QPIConsole::Magenta)) fcol = Qt::magenta;
	if (f.testFlag(QPIConsole::Cyan)) fcol = Qt::cyan;
	if (f.testFlag(QPIConsole::White)) fcol = Qt::white;
	if (f.testFlag(QPIConsole::Lighter)) fcol = fcol.lighter(150);
	if (f.testFlag(QPIConsole::Darker)) fcol = fcol.darker(150);

	if (f.testFlag(QPIConsole::BackBlack)) bcol = Qt::black;
	if (f.testFlag(QPIConsole::BackRed)) bcol = Qt::red;
	if (f.testFlag(QPIConsole::BackGreen)) bcol = Qt::green;
	if (f.testFlag(QPIConsole::BackYellow)) bcol = Qt::yellow;
	if (f.testFlag(QPIConsole::BackBlue)) bcol = Qt::blue;
	if (f.testFlag(QPIConsole::BackMagenta)) bcol = Qt::magenta;
	if (f.testFlag(QPIConsole::BackCyan)) bcol = Qt::cyan;
	if (f.testFlag(QPIConsole::BackWhite)) bcol = Qt::white;
	if (f.testFlag(QPIConsole::BackLighter)) bcol = bcol.lighter(150);
	//if (f.testFlag(QPIConsole::BackDarker)) bcol = bcol.darker(150);
	
	if (f.testFlag(QPIConsole::Inverse)) {
		QColor tc = fcol;
		fcol = bcol;
		bcol = tc;
	}
	
	l->setFont(fnt);
	QPalette pal = palette();
	pal.setColor(QPalette::WindowText, fcol);
	pal.setColor(QPalette::Window, bcol);
	l->setPalette(pal);
	l->setAutoFillBackground(bcol != QColor(0xFFFFFF));
}


bool QPIConsole::removeTab(uint index) {
	if (int(index) >= tabs.size()) return false;
	delete QTabWidget::widget(index);
	tabs.remove(index);
	return true;
}


bool QPIConsole::setTab(uint index) {
	if (int(index) >= tabs.size()) return false;
	setCurrentIndex(index);
	return true;
}


bool QPIConsole::renameTab(uint index, const QString & new_name) {
	if (int(index) >= tabs.size()) return false;
	if (tabs[index].name == new_name) return true;
	setTabText(index, new_name);
	tabs[index].name = new_name;
	return true;
}


void QPIConsole::setTabEnabled(int index, bool on) {
	if (int(index) >= tabs.size()) return;
	if (isTabEnabled(index) == on) return;
	QTabWidget::setTabEnabled(index, on);
}
