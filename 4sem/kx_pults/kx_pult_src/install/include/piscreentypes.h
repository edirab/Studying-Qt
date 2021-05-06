/*! \file piscreentypes.h
 * \brief Types for PIScreen
*/
/*
    PIP - Platform Independent Primitives
    Types for PIScreen
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

#ifndef PISCREENTYPES_H
#define PISCREENTYPES_H

#include "pivariant.h"

class PIScreenTile;

namespace PIScreenTypes {
	
	//! Color for chars or background
	enum Color {
		Default /** Default */,
		Black /** Black */,
		Red /** Red */,
		Green /** Green */,
		Blue /** Blue */,
		Cyan /** Cyan */,
		Magenta /** Magenta */,
		Yellow /** Yellow */,
		White /** White */,
		Transparent /** Save previous color */
	};
	
	//! Flags for chars
	enum CharFlag {
		Bold /** Bold or bright */ = 0x1,
		Blink /** Blink text */ = 0x2,
		Underline /** Underline text */ = 0x4,
		Inverse = 0x08
	};
	
	//! Alignment
	enum Alignment {
		Left /** Left */ ,
		Center /** Center */ ,
		Right /** Right */
	};
	
	//! Size policy
	enum SizePolicy {
		Fixed /** Fixed size */ ,
		Preferred /** Preferred size */ ,
		Expanding /** Maximum available size */ ,
		Ignore /** Ignore layout logic */
	};
	
	//! Direction
	enum Direction {
		Horizontal /** Horizontal */ ,
		Vertical /** Vertical */
	};

	//! Position
	enum Position {
		//Left /** Left */ ,
		//Right /** Right */ ,
		//Top /** Top */ ,
		//Bottom /** Bottom */
	};

	//! Focus flags
	enum FocusFlag {
		CanHasFocus /** Tile can has focus */ = 0x1,
		NextByTab /** Focus passed to next tile by tab key */ = 0x2,
		NextByArrowsHorizontal /** Focus passed to next tile by arrow keys left or right */ = 0x4,
		NextByArrowsVertical /** Focus passed to next tile by arrow keys up or down */ = 0x8,
		NextByArrowsAll /** Focus passed to next tile by any arrow key */ = NextByArrowsHorizontal | NextByArrowsVertical
	};
	
	typedef PIFlags<CharFlag> CharFlags;
	typedef PIFlags<FocusFlag> FocusFlags;
	
	union CellFormat {
		CellFormat(uint f = 0) {raw_format = f;}
		CellFormat(Color col_char, Color col_back = Default, CharFlags flags_ = 0) {
			color_char = col_char;
			color_back = col_back;
			flags = flags_;
		}
		uint raw_format;
		struct {
			uchar color_char;
			uchar color_back;
			ushort flags;
		};
		bool operator ==(const CellFormat & c) const {return raw_format == c.raw_format;}
		bool operator !=(const CellFormat & c) const {return raw_format != c.raw_format;}
	};
	
	struct Cell {
		Cell(PIChar c = PIChar(' '), CellFormat f = CellFormat()) {symbol = c; format = f;}
		CellFormat format;
		PIChar symbol;
		bool operator ==(const Cell & c) const {return format == c.format && symbol == c.symbol;}
		bool operator !=(const Cell & c) const {return format != c.format || symbol != c.symbol;}
		Cell & operator =(const Cell & c) {
			symbol = c.symbol;
			if (c.format.color_back == Transparent) {
				format.color_char = c.format.color_char;
				format.flags = c.format.flags;
			} else format = c.format;
			return *this;
		}
	};
	
	struct TileEvent {
		TileEvent(int t = -1, const PIVariant & d = PIVariant()): type(t), data(d) {}
		int type;
		PIVariant data;
	};
	
	class PIScreenBase {
	public:
		PIScreenBase() {}
		virtual ~PIScreenBase() {}
		virtual void tileEventInternal(PIScreenTile * , TileEvent) {}
		virtual void tileRemovedInternal(PIScreenTile * ) {}
		virtual void tileSetFocusInternal(PIScreenTile * ) {}
	};

}


inline PIByteArray & operator <<(PIByteArray & s, const PIScreenTypes::Cell & v) {s << v.symbol << v.format.raw_format; return s;}

inline PIByteArray & operator >>(PIByteArray & s, PIScreenTypes::Cell & v) {s >> v.symbol >> v.format.raw_format; return s;}


#endif // PISCREENTYPES_H
