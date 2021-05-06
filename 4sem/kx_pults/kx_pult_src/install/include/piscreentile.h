/*! \file piscreentile.h
 * \brief Basic PIScreen tile
*/
/*
    PIP - Platform Independent Primitives
    Basic PIScreen tile
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

#ifndef PISCREENTILE_H
#define PISCREENTILE_H

#include "piscreentypes.h"
#include "pikbdlistener.h"

class PIScreenDrawer;

class PIP_EXPORT PIScreenTile: public PIObject {
	friend class PIScreen;
	PIOBJECT_SUBCLASS(PIScreenTile, PIObject)
public:
	PIScreenTile(const PIString & n = PIString(), PIScreenTypes::Direction d = PIScreenTypes::Vertical, PIScreenTypes::SizePolicy p = PIScreenTypes::Preferred);
	virtual ~PIScreenTile();
	
	void addTile(PIScreenTile * t);
	void takeTile(PIScreenTile * t);
	void removeTile(PIScreenTile * t);
	PIScreenTile * parentTile() const {return parent;}
	PIVector<PIScreenTile * > children(bool only_visible = false);
	void show() {visible = true;}
	void hide() {visible = false;}
	void setFocus();
	bool hasFocus() const {return has_focus;}
	void setMargins(int m) {marginLeft = marginRight = marginTop = marginBottom = m;}
	void setMargins(int l, int r, int t, int b) {marginLeft = l; marginRight = r; marginTop = t; marginBottom = b;}

	int x() const {return x_;}
	int y() const {return y_;}
	int width() const {return width_;}
	int height() const {return height_;}
	
	PIScreenTypes::Direction direction;
	PIScreenTypes::SizePolicy size_policy;
	PIScreenTypes::FocusFlags focus_flags;
	PIScreenTypes::CellFormat back_format;
	PIChar back_symbol;
	int minimumWidth, minimumHeight;
	int maximumWidth, maximumHeight;
	int marginLeft, marginRight, marginTop, marginBottom;
	int spacing;
	bool visible;
	
protected:
	
	//! Returns desired tile size in "w" and "h"
	virtual void sizeHint(int & w, int & h) const;
	
	//! Tile has been resized to "w"x"h"
	virtual void resizeEvent(int w, int h) {}
	
	//! Draw tile with drawer "d" in world-space coordinates
	virtual void drawEvent(PIScreenDrawer * d) {}
	
	//! Return "true" if you process key
	virtual bool keyEvent(PIKbdListener::KeyEvent key) {return false;}
	
	void raiseEvent(PIScreenTypes::TileEvent e);
	void setScreen(PIScreenTypes::PIScreenBase * s);
	void deleteChildren();
	void drawEventInternal(PIScreenDrawer * d);
	void layout();
	bool needLayout() {return size_policy != PIScreenTypes::Ignore;}
	
	PIVector<PIScreenTile * > tiles;
	PIScreenTile * parent;
	PIScreenTypes::PIScreenBase * screen;
	int x_, y_, width_, height_;
	bool has_focus;
	
private:
	int pw, ph;
	
};


#endif // PISCREENTILE_H
