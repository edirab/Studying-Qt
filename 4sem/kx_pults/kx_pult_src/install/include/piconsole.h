/*! \file piconsole.h
 * \brief Console output class
*/
/*
	PIP - Platform Independent Primitives
	Console output/input
	Copyright (C) 2016  Ivan Pelipenko peri4ko@yandex.ru

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PICONSOLE_H
#define PICONSOLE_H

#include "pikbdlistener.h"
#include "pitimer.h"

class PIProtocol;
class PIDiagnostics;
class PISystemMonitor;
class PIPeer;

class PIP_EXPORT PIConsole: public PIThread
{
	PIOBJECT_SUBCLASS(PIConsole, PIThread)
public:

		//! Constructs %PIConsole with key handler "slot" and if "startNow" start it
		explicit PIConsole(bool startNow = true, PIKbdListener::KBFunc slot = 0);

	~PIConsole();


	//! Variables output format
	enum Format {
		Normal /** Default console format */ = 0x01,
		Bold /** Bold text */ = 0x02,
		Faint = 0x04,
		Italic = 0x08,
		Underline /** Underlined text */ = 0x10,
		Blink /** Blinked text */ = 0x20,
		Inverse /** Swap text and background colors */ = 0x40,
		Black /** Black text */ = 0x100,
		Red /** Red text */ = 0x200,
		Green /** Green text */ = 0x400,
		Yellow /** Yellow text */ = 0x800,
		Blue /** Blue text */ = 0x1000,
		Magenta /** Magenta text */ = 0x2000,
		Cyan /** Cyan text */ = 0x4000,
		White /** White text */ = 0x8000,
		BackBlack /** Black background */ = 0x10000,
		BackRed /** Red background */ = 0x20000,
		BackGreen /** Green background */ = 0x40000,
		BackYellow /** Yellow background */ = 0x80000,
		BackBlue /** Blue background */ = 0x100000,
		BackMagenta /** Magenta background */ = 0x200000,
		BackCyan /** Cyan background */ = 0x400000,
		BackWhite /** White background */ = 0x800000,
		Dec /** Decimal base for integers */ = 0x1000000,
		Hex /** Hexadecimal base for integers */ = 0x2000000,
		Oct /** Octal base for integers */ = 0x4000000,
		Bin /** Binary base for integers */ = 0x8000000,
		Scientific /** Scientific representation of floats */ = 0x10000000,
		SystemTimeSplit /** PISystemTime split representation (* s, * ns) */ = 0x20000000,
		SystemTimeSeconds /** PISystemTime seconds representation (*.* s) */ = 0x40000000
	};

	//! Column labels alignment
	enum Alignment {
		Nothing /** No alignment */ ,
		Left /** Labels align left and variables align left */ ,
		Right /** Labels align right and variables align left */
	};

	typedef PIFlags<PIConsole::Format> FormatFlags;

	//! Add to current tab to column "column" string "name" with format "format"
	void addString(const PIString & name, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const PIString * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const char * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const bool * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const short * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const int * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const long * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const llong * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const uchar * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const ushort * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const uint * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const ulong * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const ullong * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const float * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const double * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" variable with label "name", pointer "ptr" and format "format"
	void addVariable(const PIString & name, const PISystemTime * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	void addVariable(const PIString & name, const PIProtocol * ptr, int column = 1, FormatFlags format = PIConsole::Normal);
	void addVariable(const PIString & name, const PIDiagnostics * ptr, int column = 1, FormatFlags format = PIConsole::Normal);
	void addVariable(const PIString & name, const PISystemMonitor * ptr, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" bits field with label "name", pointer "ptr" and format "format"
	void addBitVariable(const PIString & name, const void * ptr, int fromBit, int bitsCount, int column = 1, FormatFlags format = PIConsole::Normal);

	//! Add to current tab to column "column" "count" empty lines
	void addEmptyLine(int column = 1, uint count = 1);

	PIString getString(int x, int y);
	short getShort(int x, int y) {return getString(x, y).toShort();}
	int getInt(int x, int y) {return getString(x, y).toInt();}
	float getFloat(int x, int y) {return getString(x, y).toFloat();}
	double getDouble(int x, int y) {return getString(x, y).toDouble();}
	PIString getString(const PIString & name);
	short getShort(const PIString & name) {return getString(name).toShort();}
	int getInt(const PIString & name) {return getString(name).toInt();}
	float getFloat(const PIString & name) {return getString(name).toFloat();}
	double getDouble(const PIString & name) {return getString(name).toDouble();}


	//! Returns tabs count
	uint tabsCount() const {return tabs.size();}

	//! Returns current tab name
	PIString currentTab() const {return tabs[cur_tab].name;}

	//! Returns current tab index
	int currentTabIndex() const {return cur_tab;}

	//! Add new tab with name "name", bind key "bind_key" and returns this tab index
	int addTab(const PIString & name, char bind_key = 0);

	//! Remove tab with index "index"
	void removeTab(uint index);

	//! Remove tab with name "name"
	void removeTab(const PIString & name);

	//! Clear content of tab with index "index"
	void clearTab(uint index);

	//! Clear content of tab with name "name"
	void clearTab(const PIString & name);

	//! Set current tab to tab with index "index", returns if tab exists
	bool setTab(uint index);

	//! Set current tab to tab with name "name", returns if tab exists
	bool setTab(const PIString & name);

	//! Set tab with index "index" bind key to "bind_key", returns if tab exists
	bool setTabBindKey(uint index, char bind_key);

	//! Set tab with name "name" bind key to "bind_key", returns if tab exists
	bool setTabBindKey(const PIString & name, char bind_key);

	//! Remove all tabs and if "clearScreen" clear the screen
	void clearTabs(bool clearScreen = true) {if (clearScreen && isRunning()) {toUpperLeft(); clearScreenLower();} tabs.clear();}


	//! Set custom status text of current tab to "str"
	void addCustomStatus(const PIString & str) {tabs[cur_tab].status = str;}

	//! Clear custom status text of current tab
	void clearCustomStatus() {tabs[cur_tab].status.clear();}

	//! Returns default alignment
	Alignment defaultAlignment() const {return def_align;}

	//! Set default alignment to "align"
	void setDefaultAlignment(Alignment align) {def_align = align;}

	//! Set column "col" alignment to "align"
	void setColumnAlignment(int col, Alignment align) {if (col < 0 || col >= columns().size_s()) return; column(col).alignment = align;}

	//! Set all columns of all tabs alignment to "align"
	void setColumnAlignmentToAll(Alignment align) {piForeach (Tab & i, tabs) piForeach (Column & j, i.columns) j.alignment = align; fillLabels();}


	//! Directly call function from \a PIKbdListener
	void enableExitCapture(char key = 'Q') {listener->enableExitCapture(key);}

	//! Directly call function from \a PIKbdListener
	void disableExitCapture() {listener->disableExitCapture();}

	//! Directly call function from \a PIKbdListener
	bool exitCaptured() const {return listener->exitCaptured();}

	//! Directly call function from \a PIKbdListener
	char exitKey() const {return listener->exitKey();}


	int windowWidth() const {return width;}
	int windowHeight() const {return height;}

	PIString fstr(FormatFlags f);
	void update();
	void pause(bool yes) {pause_ = yes;}

	// Server functions
	void startServer(const PIString & name);
	void stopPeer();
	bool isServerStarted() const {return peer != 0;}
	PIStringList clients() const;

	// Client functions
	void listenServers();
	PIStringList availableServers() const;
	PIString selectedServer() const {return server_name;}
	void connectToServer(const PIString & name);
	void disconnect();
	bool isConnected() const {return state == Connected;}

#ifdef WINDOWS
	void toUpperLeft();
	void moveRight(int n = 1);
	void moveLeft(int n = 1);
	void moveTo(int x = 0, int y = 0);
	void clearScreen();
	void clearScreenLower();
	void clearLine();
	void newLine();
	void hideCursor();
	void showCursor();
#else
	void toUpperLeft() {printf("\e[H");}
	void moveRight(int n = 1) {if (n > 0) printf("\e[%dC", n);}
	void moveLeft(int n = 1) {if (n > 0) printf("\e[%dD", n);}
	void moveTo(int x = 0, int y = 0) {printf("\e[%d;%dH", y, x);}
	void clearScreen() {couts(fstr(Normal)); printf("\e[H\e[J");}
	void clearScreenLower() {couts(fstr(Normal)); printf("\e[J");}
	void clearLine() {printf("\e[K");}
	void newLine() {printf("\eE");}
	void hideCursor() {printf("\e[?25l");}
	void showCursor() {printf("\e[?25h");}
#endif

	EVENT_HANDLER0(void, clearVariables) {clearVariables(true);}
	EVENT_HANDLER1(void, clearVariables, bool, clearScreen);

	EVENT_HANDLER0(void, waitForFinish) {WAIT_FOR_EXIT}
	EVENT_HANDLER0(void, start) {start(false);}
	EVENT_HANDLER1(void, start, bool, wait) {PIThread::start(40); if (wait) waitForFinish();}
	EVENT_HANDLER0(void, stop) {stop(false);}
	EVENT_HANDLER1(void, stop, bool, clear);

	EVENT2(keyPressed, PIKbdListener::KeyEvent, key, void * , data)

	//! \handlers
	//! \{

	//! \fn void waitForFinish()
	//! \brief block until finished (exit key will be pressed)

	//! \fn void clearVariables(bool clearScreen = true)
	//! \brief Remove all columns at current tab and if "clearScreen" clear the screen

	//! \fn void start(bool wait = false)
	//! \brief Start console output and if "wait" block until finished (exit key will be pressed)

	//! \fn void stop(bool clear = false)
	//! \brief Stop console output and if "clear" clear the screen

	//! \}
	//! \events
	//! \{

	//! \fn void keyPressed(PIKbdListener::KeyEvent key, void * data)
	//! \brief Raise on key "key" pressed, "data" is pointer to %PIConsole object

	//! \}

	private:
		void begin();
	void run();
	void fillLabels();
	void status();
	void checkColumn(uint col) {while (columns().size() < col) columns().push_back(Column(def_align));}
	int bitsValue(const void * src, int offset, int count) const;
	const char * toBin(const void * d, int s);
	inline void printLine(const PIString & str, int dx = 0, FormatFlags format = PIConsole::Normal);
	inline int printValue(const PIString & str, FormatFlags format = PIConsole::Normal);
	inline int printValue(const char * str, FormatFlags format = PIConsole::Normal);
	inline int printValue(const bool value, FormatFlags format = PIConsole::Normal);
	inline int printValue(const int value, FormatFlags format = PIConsole::Normal);
	inline int printValue(const long value, FormatFlags format = PIConsole::Normal);
	inline int printValue(const llong value, FormatFlags format = PIConsole::Normal);
	inline int printValue(const float value, FormatFlags format = PIConsole::Normal);
	inline int printValue(const double value, FormatFlags format = PIConsole::Normal);
	inline int printValue(const char value, FormatFlags format = PIConsole::Normal);
	inline int printValue(const short value, FormatFlags format = PIConsole::Normal);
	inline int printValue(const uchar value, FormatFlags format = PIConsole::Normal);
	inline int printValue(const ushort value, FormatFlags format = PIConsole::Normal);
	inline int printValue(const uint value, FormatFlags format = PIConsole::Normal);
	inline int printValue(const ulong value, FormatFlags format = PIConsole::Normal);
	inline int printValue(const ullong value, FormatFlags format = PIConsole::Normal);
	inline int printValue(const PISystemTime & value, FormatFlags format = PIConsole::Normal);
	static void key_event(PIKbdListener::KeyEvent key, void * t);

	struct Variable {
		Variable() {nx = ny = type = offset = bitFrom = bitCount = size = 0; format = Normal; remote = false; ptr = 0; id = 1;}
		Variable(const Variable & src) {remote = src.remote; name = src.name; format = src.format; type = src.type; offset = src.offset; size = src.size;
										bitFrom = src.bitFrom; bitCount = src.bitCount; ptr = src.ptr; nx = src.nx; ny = src.ny; rdata = src.rdata; id = src.id;}
		bool isEmpty() const {return (remote ? false : ptr == 0);}
		const void * data() {return (remote ? rdata.data() : ptr);}
		void writeData(PIByteArray & ba) {
			if (remote) ba << rdata;
			else {
				if (type == 0) ba << (*(PIString * )ptr);
				else ba << PIByteArray::RawData(ptr, size);
			}
		}
		PIString name;
		FormatFlags format;
		int nx;
		int ny;
		int type;
		int offset;
		int bitFrom;
		int bitCount;
		int size;
		int id;
		bool remote;
		const void * ptr;
		PIByteArray rdata;
		void operator =(const Variable & src) {remote = src.remote; name = src.name; format = src.format; type = src.type; offset = src.offset; size = src.size;
											   bitFrom = src.bitFrom; bitCount = src.bitCount; ptr = src.ptr; nx = src.nx; ny = src.ny; rdata = src.rdata; id = src.id;}
	};

	struct VariableContent {
		int id;
		PIByteArray rdata;
	};

	struct Column {
		Column(Alignment align = PIConsole::Right) {variables.reserve(32); alignment = align;}
		PIVector<Variable> variables;
		Alignment alignment;
		uint size() const {return variables.size();}
		Variable & operator [](int index) {return variables[index];}
		const Variable & operator [](int index) const {return variables[index];}
		void push_back(const Variable & v) {variables.push_back(v);}
		void operator =(const Column & src) {variables = src.variables; alignment = src.alignment;}
	};

	struct Tab {
		Tab(PIString n = "", char k = 0) {columns.reserve(8); name = n; key = k;}
		PIVector<Column> columns;
		PIString name;
		PIString status;
		char key;
	};

	enum ConnectedState {Disconnected, FetchingData, Committing, Connected};

	friend PIByteArray & operator <<(PIByteArray & ba, const PIConsole::VariableContent & v);
	friend PIByteArray & operator >>(PIByteArray & ba, PIConsole::VariableContent & v);

	friend PIByteArray & operator <<(PIByteArray & ba, const PIConsole::Variable & v);
	friend PIByteArray & operator >>(PIByteArray & ba, PIConsole::Variable & v);

	friend PIByteArray & operator <<(PIByteArray & ba, const PIConsole::Column & v);
	friend PIByteArray & operator >>(PIByteArray & ba, PIConsole::Column & v);

	friend PIByteArray & operator <<(PIByteArray & ba, const PIConsole::Tab & v);
	friend PIByteArray & operator >>(PIByteArray & ba, PIConsole::Tab & v);

	PIVector<Column> & columns() {return tabs[cur_tab].columns;}
	Column & column(int index) {return tabs[cur_tab].columns[index - 1];}
	inline int couts(const PIString & v) {return printf("%s", v.data());}
	inline int couts(const char * v) {return printf("%s", v);}
	inline int couts(const bool v);
	inline int couts(const char v);
	inline int couts(const short v);
	inline int couts(const int v);
	inline int couts(const long v);
	inline int couts(const llong v);
	inline int couts(const uchar v);
	inline int couts(const ushort v);
	inline int couts(const uint v);
	inline int couts(const ulong v);
	inline int couts(const ullong v);
	inline int couts(const float v);
	inline int couts(const double v);
	inline int couts(const PISystemTime & v);

	struct RemoteClient;

	void serverSendInfo();
	void serverSendData();
	RemoteClient & remoteClient(const PIString & fname);
	EVENT_HANDLER2(void, peerReceived, const PIString &, from, const PIByteArray &, data);
	EVENT_HANDLER2(void, peerTimer, void * , data, int, delim);
	EVENT_HANDLER1(void, peerDisconnectedEvent, const PIString &, name);

	PRIVATE_DECLARATION
	PIVector<Tab> tabs;
	PIString binstr, rstr;
	PIByteArray rba;
	Variable tv;
	PIKbdListener * listener;
	Alignment def_align;
	PIKbdListener::KBFunc ret_func;
	int width, height, pwidth, pheight, ret, col_wid, num_format, systime_format;
	uint max_y;
	int vid;
	uint cur_tab, col_cnt;

	PIPeer * peer;
	PITimer peer_timer;
	PITimeMeasurer peer_tm;
	PIString server_name;
	bool server_mode, pause_;
	ConnectedState state;

	/*struct RemoteData {
		RemoteData() {msg_count = msg_rec = msg_send = 0;}
		void clear() {msg_count = msg_rec = msg_send = 0; data.clear();}
		bool isEmpty() const {return msg_count == 0;}
		bool isReadyRec() const {return msg_count == msg_rec;}
		bool isReadySend() const {return msg_count == msg_send;}
		void setData(const PIByteArray & ba) {data = ba; msg_rec = msg_send = 0; msg_count = (data.size_s() - 1) / 4096 + 1;}
		PIByteArray data;
		int msg_count;
		int msg_rec;
		int msg_send;
	};*/

	struct RemoteClient {
		RemoteClient(const PIString & n = "") {name = n; state = Disconnected;}
		PIString name;
		ConnectedState state;
	};

	PIVector<RemoteClient> remote_clients;

};

inline PIByteArray & operator <<(PIByteArray & ba, const PIConsole::VariableContent & v) {ba << v.id << v.rdata; return ba;}
inline PIByteArray & operator >>(PIByteArray & ba, PIConsole::VariableContent & v) {ba >> v.id; ba >> v.rdata; return ba;}

inline PIByteArray & operator <<(PIByteArray & ba, const PIConsole::Variable & v) {ba << v.name << v.id << (int)v.format << v.type << v.size << v.bitFrom << v.bitCount; return ba;}
inline PIByteArray & operator >>(PIByteArray & ba, PIConsole::Variable & v) {ba >> v.name >> v.id >> (int & )v.format >> v.type >> v.size >> v.bitFrom >> v.bitCount; return ba;}

inline PIByteArray & operator <<(PIByteArray & ba, const PIConsole::Column & v) {ba << (int)v.alignment << v.variables; return ba;}
inline PIByteArray & operator >>(PIByteArray & ba, PIConsole::Column & v) {ba >> (int & )v.alignment >> v.variables; return ba;}

inline PIByteArray & operator <<(PIByteArray & ba, const PIConsole::Tab & v) {ba << v.name << v.status << (uchar)v.key << v.columns; return ba;}
inline PIByteArray & operator >>(PIByteArray & ba, PIConsole::Tab & v) {ba >> v.name >> v.status >> (uchar&)v.key >> v.columns; return ba;}

#endif // PICONSOLE_H
