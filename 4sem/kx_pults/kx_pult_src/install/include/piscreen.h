/*! \file piscreen.h
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

#ifndef PISCREEN_H
#define PISCREEN_H

#include "piscreentile.h"
#include "piscreendrawer.h"


class PIP_EXPORT PIScreen: public PIThread, public PIScreenTypes::PIScreenBase
{
	PIOBJECT_SUBCLASS(PIScreen, PIThread)
	class SystemConsole;
public:
	
	//! Constructs %PIScreen with key handler "slot" and if "startNow" start it
	PIScreen(bool startNow = true, PIKbdListener::KBFunc slot = 0);
	
	~PIScreen();
	
	//! Directly call function from \a PIKbdListener
	void enableExitCapture(int key = 'Q') {listener->enableExitCapture(key);}
	
	//! Directly call function from \a PIKbdListener
	void disableExitCapture() {listener->disableExitCapture();}
	
	//! Directly call function from \a PIKbdListener
	bool exitCaptured() const {return listener->exitCaptured();}
	
	//! Directly call function from \a PIKbdListener
	int exitKey() const {return listener->exitKey();}
	
	int windowWidth() const {return console.width;}
	int windowHeight() const {return console.height;}
	
	PIScreenTile * rootTile() {return &root;}
	PIScreenTile * tileByName(const PIString & name);
	
	void setDialogTile(PIScreenTile * t);
	PIScreenTile * dialogTile() const {return tile_dialog;}
	
	PIScreenDrawer * drawer() {return &drawer_;}
	void clear() {drawer_.clear();}
	
	EVENT_HANDLER0(void, waitForFinish);
	EVENT_HANDLER0(void, start) {start(false);}
	EVENT_HANDLER1(void, start, bool, wait) {PIThread::start(40); if (wait) waitForFinish();}
	EVENT_HANDLER0(void, stop) {stop(false);}
	EVENT_HANDLER1(void, stop, bool, clear);
	
	EVENT2(keyPressed, PIKbdListener::KeyEvent, key, void * , data)
	EVENT2(tileEvent, PIScreenTile * , tile, PIScreenTypes::TileEvent, e)
	
//! \handlers
//! \{
	
	//! \fn void waitForFinish()
	//! \brief block until finished (exit key will be pressed)
	
	//! \fn void start(bool wait = false)
	//! \brief Start console output and if "wait" block until finished (exit key will be pressed)
	
	//! \fn void stop(bool clear = false)
	//! \brief Stop console output and if "clear" clear the screen
	
//! \}
//! \events
//! \{
	
	//! \fn void keyPressed(PIKbdListener::KeyEvent key, void * data)
	//! \brief Raise on key "key" pressed, "data" is pointer to %PIConsole object
	
	//! \fn void tileEvent(PIScreenTile * tile, PIScreenTypes::TileEvent e)
	//! \brief Raise on some event "e" from tile "tile"
	
//! \}
	
private:
	class SystemConsole {
	public:
		SystemConsole();
		~SystemConsole();
		void begin();
		void end();
		void prepare();
		void clear();
		void print();
		void resize(int w, int h);
#ifdef WINDOWS
		void getWinCurCoord();
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
		WORD attributes(const PIScreenTypes::Cell & c);
#else
		void toUpperLeft() {printf("\e[H");}
		void moveTo(int x = 0, int y = 0) {printf("\e[%d;%dH", y + 1, x + 1);}
		void hideCursor() {printf("\e[?25l");}
		void showCursor() {printf("\e[?25h");}
		void clearScreen() {printf("\e[0m\e[H\e[J");}
		void clearScreenLower() {printf("\e[0m\e[J");}
		PIString formatString(const PIScreenTypes::Cell & c);
#endif
		PRIVATE_DECLARATION
		int width, height, pwidth, pheight;
		PIVector<PIVector<PIScreenTypes::Cell> > cells, pcells, dcells;
	};

	void begin();
	void run();
	void end();
	void key_event(PIKbdListener::KeyEvent key);
	static void key_eventS(PIKbdListener::KeyEvent key, void * t) {((PIScreen*)t)->key_event(key);}
	PIVector<PIScreenTile*> tiles() {return root.children();}
	bool nextFocus(PIScreenTile * rt, PIKbdListener::KeyEvent key = PIKbdListener::KeyEvent());
	void tileEventInternal(PIScreenTile * t, PIScreenTypes::TileEvent e);
	void tileRemovedInternal(PIScreenTile * t);
	void tileSetFocusInternal(PIScreenTile * t);

	SystemConsole console;
	PIScreenDrawer drawer_;
	PIKbdListener * listener;
	PIKbdListener::KBFunc ret_func;
	PIScreenTile root;
	PIScreenTile * tile_focus, * tile_dialog;

};


#endif // PISCREEN_H
