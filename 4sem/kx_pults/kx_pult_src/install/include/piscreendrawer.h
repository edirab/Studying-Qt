/*! \file piscreendrawer.h
 * \brief Drawer for PIScreen
*/
/*
    PIP - Platform Independent Primitives
    Drawer for PIScreen
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

#ifndef PISCREENDRAWER_H
#define PISCREENDRAWER_H

#include "piscreentypes.h"
#include "pistring.h"

class PIP_EXPORT PIScreenDrawer
{
	friend class PIScreen;
	PIScreenDrawer(PIVector<PIVector<PIScreenTypes::Cell> > & c);
public:
	enum ArtChar {LineVertical = 1, LineHorizontal, Cross, CornerTopLeft, CornerTopRight, CornerBottomLeft, CornerBottomRight};
	
	void clear();
	void clearRect(int x0, int y0, int x1, int y1) {fillRect(x0, y0, x1, y1, ' ');}
	void drawPixel(int x, int y, const PIChar & c, PIScreenTypes::Color col_char = PIScreenTypes::Default, PIScreenTypes::Color col_back = PIScreenTypes::Default, PIScreenTypes::CharFlags flags_char = 0);
	void drawLine(int x0, int y0, int x1, int y1, const PIChar & c, PIScreenTypes::Color col_char = PIScreenTypes::Default, PIScreenTypes::Color col_back = PIScreenTypes::Default, PIScreenTypes::CharFlags flags_char = 0);
	void drawRect(int x0, int y0, int x1, int y1, const PIChar & c, PIScreenTypes::Color col_char = PIScreenTypes::Default, PIScreenTypes::Color col_back = PIScreenTypes::Default, PIScreenTypes::CharFlags flags_char = 0);
	void drawFrame(int x0, int y0, int x1, int y1, PIScreenTypes::Color col_char = PIScreenTypes::Default, PIScreenTypes::Color col_back = PIScreenTypes::Default, PIScreenTypes::CharFlags flags_char = 0);
	void drawText(int x, int y, const PIString & s, PIScreenTypes::Color col_char = PIScreenTypes::Default, PIScreenTypes::Color col_back = PIScreenTypes::Transparent, PIScreenTypes::CharFlags flags_char = 0);
	void fillRect(int x0, int y0, int x1, int y1, const PIChar & c, PIScreenTypes::Color col_char = PIScreenTypes::Default, PIScreenTypes::Color col_back = PIScreenTypes::Default, PIScreenTypes::CharFlags flags_char = 0);
	void fillRect(int x0, int y0, int x1, int y1, PIVector<PIVector<PIScreenTypes::Cell> > & content);
	
	PIChar artChar(const ArtChar type) const {return arts_.value(type, PIChar(' '));}

	static void clear(PIVector<PIVector<PIScreenTypes::Cell> > & cells);

private:
	PIVector<PIVector<PIScreenTypes::Cell> > & cells;
	int width, height;
	PIMap<ArtChar, PIChar> arts_;
	
};


#endif // PISCREENDRAWER_H
