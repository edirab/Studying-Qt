/*! \file piscreentiles.h
 * \brief Various tiles for PIScreen
*/
/*
    PIP - Platform Independent Primitives
    Various tiles for PIScreen
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

#ifndef PISCREENTILES_H
#define PISCREENTILES_H

#include "piscreentile.h"


class PIP_EXPORT TileSimple: public PIScreenTile {
public:
	typedef PIPair<PIString, PIScreenTypes::CellFormat> Row;
	TileSimple(const PIString & n = PIString());
	TileSimple(const Row & r);
	PIVector<Row> content;
	PIScreenTypes::Alignment alignment;
protected:
	void sizeHint(int & w, int & h) const;
	void drawEvent(PIScreenDrawer * d);
};


class TileList;

class PIP_EXPORT TileScrollBar: public PIScreenTile {
	friend class TileList;
public:
	TileScrollBar(const PIString & n = PIString());
	void setMinimum(int v);
	void setMaximum(int v);
	void setValue(int v);
	int minimum() const {return minimum_;}
	int maximum() const {return maximum_;}
	int value() const {return value_;}
	int thickness;
protected:
	void _check();
	void sizeHint(int & w, int & h) const;
	void drawEvent(PIScreenDrawer * d);
	int minimum_, maximum_, value_;
	PIChar line_char;
};


class PIP_EXPORT TileList: public PIScreenTile {
public:
	TileList(const PIString & n = PIString());
	~TileList();
	enum SelectionMode {
		NoSelection,
		SingleSelection,
		MultiSelection
	};
	enum EventType {
		SelectionChanged,
		RowPressed
	};
	typedef PIPair<PIString, PIScreenTypes::CellFormat> Row;
	PIDeque<Row> content;
	PIScreenTypes::Alignment alignment;
	SelectionMode selection_mode;
	PISet<int> selected;
	int lhei, cur, offset;
protected:
	void sizeHint(int & w, int & h) const;
	void resizeEvent(int w, int h);
	void drawEvent(PIScreenDrawer * d);
	bool keyEvent(PIKbdListener::KeyEvent key);
	TileScrollBar * scroll;
};


class PIP_EXPORT TileButton: public PIScreenTile {
public:
	TileButton(const PIString & n = PIString());
	enum EventType {
		ButtonClicked
	};
	PIScreenTypes::CellFormat format;
	PIString text;
protected:
	void sizeHint(int & w, int & h) const;
	void drawEvent(PIScreenDrawer * d);
	bool keyEvent(PIKbdListener::KeyEvent key);
};




class PIP_EXPORT TileButtons: public PIScreenTile {
public:
	TileButtons(const PIString & n = PIString());
	enum EventType {
		ButtonSelected
	};
	typedef PIPair<PIString, PIScreenTypes::CellFormat> Button;
	PIScreenTypes::Alignment alignment;
	PIVector<Button> content;
	int cur;
protected:
	void sizeHint(int & w, int & h) const;
	void drawEvent(PIScreenDrawer * d);
	bool keyEvent(PIKbdListener::KeyEvent key);
};


class PIP_EXPORT TileCheck: public PIScreenTile {
public:
	TileCheck(const PIString & n = PIString());
	enum EventType {
		Toggled
	};
	PIScreenTypes::CellFormat format;
	PIString text;
	bool toggled;
protected:
	void sizeHint(int & w, int & h) const;
	void drawEvent(PIScreenDrawer * d);
	bool keyEvent(PIKbdListener::KeyEvent key);
};


class PIP_EXPORT TileProgress: public PIScreenTile {
public:
	TileProgress(const PIString & n = PIString());
	PIScreenTypes::CellFormat format;
	PIString prefix;
	PIString suffix;
	double maximum;
	double value;
protected:
	void sizeHint(int & w, int & h) const;
	void drawEvent(PIScreenDrawer * d);
};


class PIP_EXPORT TilePICout: public TileList {
public:
	TilePICout(const PIString & n = PIString());
	PIScreenTypes::CellFormat format;
	int max_lines;
protected:
	void drawEvent(PIScreenDrawer * d);
	bool keyEvent(PIKbdListener::KeyEvent key);
};


class PIP_EXPORT TileInput: public PIScreenTile {
public:
	TileInput(const PIString & n = PIString());
	/*enum EventType {
		EditFinished
	};*/
	PIScreenTypes::CellFormat format;
	PIString text;
	int max_length;
protected:
	void sizeHint(int & w, int & h) const;
	void drawEvent(PIScreenDrawer * d);
	bool keyEvent(PIKbdListener::KeyEvent key);
	void reserCursor();
	int cur, offset;
	bool inv;
	PITimeMeasurer tm_blink;
};


class PIP_EXPORT TileTabs: public PIScreenTile {
public:
	TileTabs(const PIString & n = PIString());
	enum EventType {
		TabChanged
	};
	struct Tab {
		Tab(const PIString & l = PIString(), int k = 0, PIScreenTile * t = 0) {
			label = l;
			key = k;
			tile = t;
		}
		PIString label;
		int key;
		PIScreenTile * tile;
	};
	PIScreenTypes::CellFormat format_tab;
	PIScreenTypes::CellFormat format_tab_selected;
	PIDeque<Tab> tabs;
protected:
	void sizeHint(int & w, int & h) const;
	void drawEvent(PIScreenDrawer * d);
	bool keyEvent(PIKbdListener::KeyEvent key);
	void selectTab(int index);
	int cur;
};


#endif // PISCREENTILES_H
