#ifndef QPICONSOLE_H
#define QPICONSOLE_H

#include <QTabWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QFormLayout>
#include <QSpacerItem>
#include <QDebug>

typedef long long llong;
typedef unsigned char uchar;
typedef unsigned short int ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;
typedef long double ldouble;

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QPIConsole: public QTabWidget {
	Q_OBJECT
public:
	QPIConsole(QWidget * parent = 0);
	
	enum Format {Normal = 0x01,
				 Bold = 0x02,
				 Italic = 0x08,
				 Underline = 0x10,
				 Inverse = 0x40,
				 Black = 0x100,
				 Red = 0x200,
				 Green = 0x400,
				 Yellow = 0x800,
				 Blue = 0x1000,
				 Magenta = 0x2000,
				 Cyan = 0x4000,
				 White = 0x8000,
				 BackBlack = 0x10000,
				 BackRed = 0x20000,
				 BackGreen = 0x40000,
				 BackYellow = 0x80000,
				 BackBlue = 0x100000,
				 BackMagenta = 0x200000,
				 BackCyan = 0x400000,
				 BackWhite = 0x800000,
				 Dec = 0x1000000,
				 Hex = 0x2000000,
				 Oct = 0x4000000,
				 Bin = 0x8000000,
				 Scientific = 0x10000000,
				 Lighter = 0x20000000,
				 Darker = 0x40000000,
				 BackLighter = 0x80000000
				 };
	
	Q_DECLARE_FLAGS(Formats, Format)
	Q_FLAGS(Formats)
	Q_ENUMS(Format)
	
	Q_PROPERTY(Qt::Alignment defaultAlignment READ defaultAlignment WRITE setDefaultAlignment)

	void addString(const QString & name, int column = 1, Formats format = QPIConsole::Normal);
	void addVariable(const QString & name, const QString * ptr, int column = 1, Formats format = QPIConsole::Normal);
	void addVariable(const QString & name, const char * ptr, int column = 1, Formats format = QPIConsole::Normal);
	void addVariable(const QString & name, const bool * ptr, int column = 1, Formats format = QPIConsole::Normal);
	void addVariable(const QString & name, const short * ptr, int column = 1, Formats format = QPIConsole::Normal);
	void addVariable(const QString & name, const int * ptr, int column = 1, Formats format = QPIConsole::Normal);
	void addVariable(const QString & name, const long * ptr, int column = 1, Formats format = QPIConsole::Normal);
	void addVariable(const QString & name, const llong * ptr, int column = 1, Formats format = QPIConsole::Normal);
	void addVariable(const QString & name, const uchar * ptr, int column = 1, Formats format = QPIConsole::Normal);
	void addVariable(const QString & name, const ushort * ptr, int column = 1, Formats format = QPIConsole::Normal);
	void addVariable(const QString & name, const uint * ptr, int column = 1, Formats format = QPIConsole::Normal);
	void addVariable(const QString & name, const ulong * ptr, int column = 1, Formats format = QPIConsole::Normal);
	void addVariable(const QString & name, const ullong * ptr, int column = 1, Formats format = QPIConsole::Normal);
	void addVariable(const QString & name, const float * ptr, int column = 1, Formats format = QPIConsole::Normal);
	void addVariable(const QString & name, const double * ptr, int column = 1, Formats format = QPIConsole::Normal);
	void addBitVariable(const QString & name, const void * ptr, int fromBit, int bitCount, int column = 1, Formats format = QPIConsole::Normal);
	void addEmptyLine(int column = 1, uint count = 1);

	uint tabsCount() const {return tabs.size();}
	QString currentTab() const {return tabs[cur_tab].name;}
	int addTab(const QString & name, char bind_key = 0);
	bool removeTab(uint index);
	bool removeTab(const QString & name) {return removeTab(tabIndex(name));}
	bool renameTab(uint index, const QString & new_name);
	bool renameTab(const QString & name, const QString & new_name) {return renameTab(tabIndex(name), new_name);}
	void setTabEnabled(int index, bool on);
	bool setTab(uint index);
	bool setTab(const QString & name) {return setTab(tabIndex(name));}
	//void clearTabs(bool clearScreen = true) {if (clearScreen && isRunning()) {toUpperLeft(); clearScreenLower();} tabs.clear();}

	void addCustomStatus(const QString & str, Formats format = QPIConsole::Normal) {tabs[cur_tab].widget->setText(str); applyFormat(tabs[cur_tab].widget, format);}
	void clearCustomStatus() {tabs[cur_tab].widget->clear();}
	Qt::Alignment defaultAlignment() const {return def_align;}
	void setDefaultAlignment(Qt::Alignment align) {def_align = align;}
	void setColumnAlignment(int col, Qt::Alignment align) {if (col < 0 || col >= columns().size()) return; column(col).alignment = align;}
	void setColumnAlignmentToAll(Qt::Alignment align) {for (int i = 0; i < tabs.size(); ++i) for (int j = 0; j < tabs[i].columns.size(); ++j) tabs[i].columns[j].alignment = align;/* fillLabels();*/}

	void clearVariables() {clearVariables(true);}
	void clearVariables(bool clearScreen) {/*if (clearScreen && isRunning()) {toUpperLeft(); clearScreenLower();}*/ columns().clear();}

private:
	void timerEvent(QTimerEvent * );
	QSize sizeHint() const {return QSize(100, 100);}

	void checkColumn(int col);
	void applyFormat(QLabel * l, Formats f);
	int bitsValue(const void * src, int offset, int count) const;
	int tabIndex(const QString & n) const {for (int i = 0; i < tabs.size(); ++i) if (tabs[i].name == n) return i; return -1;}
	const QString & toBin(const void * d, int s);
	template <typename T>
	QString numIntString(T v, Formats f, int bits = 0) {
		if (f.testFlag(QPIConsole::Hex)) return "0x" + QString::number(v, 16).toUpper();
		if (f.testFlag(QPIConsole::Dec)) return QString::number(v);
		if (f.testFlag(QPIConsole::Oct)) return "0" + QString::number(v, 8);
		if (f.testFlag(QPIConsole::Bin)) return toBin(&v, bits);
		return QString::number(v);
	}
	template <typename T>
	QString numFloatString(T v, Formats f) {
		if (f.testFlag(QPIConsole::Scientific)) return QString::number(v, 'E', 5);
		return QString::number(v);
	}
	
	struct Variable {
		Variable() {label = widget = 0;}
		QString name;
		Formats format;
		int type;
		int bitFrom;
		int bitCount;
		const void * ptr;
		QLabel * label;
		QLabel * widget;
		void operator =(const Variable & src) {name = src.name; format = src.format; type = src.type;
											   bitFrom = src.bitFrom; bitCount = src.bitCount; ptr = src.ptr;}
	};

	struct Column {
		Column(Qt::Alignment align = Qt::AlignRight) {variables.reserve(16); alignment = align;}
		QVector<Variable> variables;
		Qt::Alignment alignment;
		QFormLayout * layout;
		int size() const {return variables.size();}
		Variable & operator [](int index) {return variables[index];}
		const Variable & operator [](int index) const {return variables[index];}
		void push_back(const Variable & v) {variables.push_back(v);}
		void operator =(const Column & src) {variables = src.variables; alignment = src.alignment;}
	};

	struct Tab {
		Tab(QString n = "", char k = 0) {columns.reserve(16); name = n; key = k;}
		QVector<Column> columns;
		QString name;
		char key;
		QHBoxLayout * layout;
		QLabel * widget;
	};

	QVector<Column> & columns() {return tabs[cur_tab].columns;}
	Column & column(int index) {return tabs[cur_tab].columns[index - 1];}

	Qt::Alignment def_align;
	QVector<Tab> tabs;
	QString binstr;
	Variable tv;
	int cur_tab, timer;
	
private slots:
	void tabChanged(int tab) {cur_tab = tab;}
	
public slots:
	void start(float freq = 40) {if (timer >= 0) killTimer(timer); timer = startTimer(freq > 0. ? 1000 / freq : 25);}
	void stop() {if (timer >= 0) killTimer(timer); timer = -1;}

};

Q_DECLARE_OPERATORS_FOR_FLAGS(QPIConsole::Formats)

QT_END_NAMESPACE

QT_END_HEADER

#endif // QPICONSOLE_H
