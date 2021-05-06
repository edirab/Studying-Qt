/*! \file piterminal.h
 * \brief Virtual terminal
*/
/*
    PIP - Platform Independent Primitives
	Virtual terminal
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

#ifndef PITERMINAL_H
#define PITERMINAL_H

#include "pithread.h"
#include "pikbdlistener.h"
#include "piscreentypes.h"


class PIP_EXPORT PITerminal: public PIThread
{
	PIOBJECT_SUBCLASS(PITerminal, PIThread)
public:
	
	//! Constructs %PITerminal
	PITerminal();
	
	~PITerminal();
	
	int columns() const {return size_x;}
	int rows() const {return size_y;}
	bool resize(int cols, int rows);

	void write(const PIByteArray & d);
	void write(PIKbdListener::SpecialKey k, PIKbdListener::KeyModifiers m);
	void write(PIKbdListener::KeyEvent ke);
	PIVector<PIVector<PIScreenTypes::Cell> > content();
	static bool isSpecialKey(int k);

	bool initialize();
	void destroy();
private:
	void initPrivate();
	void readConsole();
	void getCursor(int & x, int & y);
	uchar invertColor(uchar c);
	void run();
#ifndef WINDOWS
	void parseInput(const PIString & s);
	bool isCompleteEscSeq(const PIString & es);
	void applyEscSeq(PIString es);
	void moveCursor(int dx, int dy);
	int termType(const PIString & t);
#endif

	PRIVATE_DECLARATION
	int dsize_x, dsize_y;
	int size_x, size_y, cursor_x, cursor_y;
	bool cursor_blink, cursor_visible;
	PITimeMeasurer cursor_tm;
	PIVector<PIVector<PIScreenTypes::Cell> > cells;

};


#endif // PITERMINAL_H
