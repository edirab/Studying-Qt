/*! \file pichar.h
 * \brief Unicode char
*/
/*
    PIP - Platform Independent Primitives
    Unicode char
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

#ifndef PICHAR_H
#define PICHAR_H

#include "pibytearray.h"

#ifdef PIP_ICU
extern char * __syslocname__;
extern char * __sysoemname__;
#endif

class PIP_EXPORT PIChar
{
	friend class PIString;
	friend PIByteArray & operator <<(PIByteArray & s, const PIChar & v);
	friend PIByteArray & operator >>(PIByteArray & s, PIChar & v);
	friend PICout operator <<(PICout s, const PIChar & v);
public:
	//! Contructs ascii symbol
	PIChar(const char c) {ch = c; ch &= 0xFF;}

	//! Contructs 2-bytes symbol
	PIChar(const short c) {ch = c;}

	//! Contructs 4-bytes symbol
	PIChar(const int c) {ch = c;}

	//! Contructs ascii symbol
	PIChar(const uchar c) {ch = c; ch &= 0xFF;}

	//! Contructs 2-bytes symbol
	PIChar(const ushort c) {ch = c;}

	//! Default constructor. Contructs 4-bytes symbol
	PIChar(const uint c = 0) {ch = c;}

	//! Contructs symbol from no more than 4 bytes of string
	PIChar(const char * c, int * bytes = 0);

	//inline operator const int() {return static_cast<const int>(ch);}
	//inline operator const char() {return toAscii();}

	//! Copy operator
	PIChar & operator =(const char v) {ch = v; return *this;}
	/*inline PIChar & operator =(const short v) {ch = v; return *this;}
	inline PIChar & operator =(const int v) {ch = v; return *this;}
	inline PIChar & operator =(const uchar v) {ch = v; return *this;}
	inline PIChar & operator =(const ushort v) {ch = v; return *this;}
	inline PIChar & operator =(const uint v) {ch = v; return *this;}*/

	//! Compare operator
	bool operator ==(const PIChar & o) const;
	/*inline bool operator ==(const PIChar & o) const {if (o.isAscii() ^ isAscii()) return false;
													if (isAscii()) return (o.toAscii() == toAscii());
													return (o.toInt() == toInt());}
	inline bool operator ==(const char o) const {return (PIChar(o) == *this);}
	inline bool operator ==(const short o) const {return (PIChar(o) == *this);}
	inline bool operator ==(const int o) const {return (PIChar(o) == *this);}
	inline bool operator ==(const uchar o) const {return (PIChar(o) == *this);}
	inline bool operator ==(const ushort o) const {return (PIChar(o) == *this);}
	inline bool operator ==(const uint o) const {return (PIChar(o) == *this);}*/

	//! Compare operator
	bool operator !=(const PIChar & o) const {return !(o == *this);}
	/*inline bool operator !=(const char o) const {return (PIChar(o) != *this);}
	inline bool operator !=(const short o) const {return (PIChar(o) != *this);}
	inline bool operator !=(const int o) const {return (PIChar(o) != *this);}
	inline bool operator !=(const uchar o) const {return (PIChar(o) != *this);}
	inline bool operator !=(const ushort o) const {return (PIChar(o) != *this);}
	inline bool operator !=(const uint o) const {return (PIChar(o) != *this);}*/

	//! Compare operator
	bool operator >(const PIChar & o) const;

	//! Compare operator
	bool operator <(const PIChar & o) const;

	//! Compare operator
	bool operator >=(const PIChar & o) const;

	//! Compare operator
	bool operator <=(const PIChar & o) const;

	//! Return \b true if symbol is digit ('0' to '9')
	bool isDigit() const;
	
	//! Return \b true if symbol is HEX digit ('0' to '9', 'a' to 'f', 'A' to 'F')
	bool isHex() const;
	
	//! Return \b true if symbol is drawable (without space)
	bool isGraphical() const;
	
	//! Return \b true if symbol is control byte (< 32 or 127)
	bool isControl() const;
	
	//! Return \b true if symbol is in lower case
	bool isLower() const;
	
	//! Return \b true if symbol is in upper case
	bool isUpper() const;
	
	//! Return \b true if symbol is printable (with space)
	bool isPrint() const;
	
	//! Return \b true if symbol is space or tab
	bool isSpace() const;
	
	//! Return \b true if symbol is alphabetical letter
	bool isAlpha() const;
	
	//! Return \b true if symbol is ascii (< 128)
	bool isAscii() const;

	const wchar_t * toWCharPtr() const;
	
	//! Return as <tt>"char * "</tt> string
	const char * toCharPtr() const;
	
	wchar_t toWChar() const;
	char toAscii() const {return ch % 256;}
	char toConcole1Byte() const;
	ushort unicode16Code() const {return ch;}
	
	//! Return symbol in upper case
	PIChar toUpper() const;
	
	//! Return symbol in lower case
	PIChar toLower() const;
	
	static PIChar fromConsole(char c);
	static PIChar fromSystem(char c);
	static PIChar fromUTF8(const char * c);

private:
	ushort ch;

};

__PICONTAINERS_SIMPLE_TYPE__(PIChar)

//! Output operator to \c std::ostream
std::ostream & operator <<(std::ostream & s, const PIChar & v);

//! Output operator to \a PICout
PICout operator <<(PICout s, const PIChar & v);


//! Write operator to \c PIByteArray
inline PIByteArray & operator <<(PIByteArray & s, const PIChar & v) {s << v.ch; return s;}

//! Read operator from \c PIByteArray
inline PIByteArray & operator >>(PIByteArray & s, PIChar & v) {s >> v.ch; return s;}


//! Compare operator
inline bool operator ==(const char v, const PIChar & c) {return (PIChar(v) == c);}

//! Compare operator
inline bool operator >(const char v, const PIChar & c) {return (PIChar(v) > c);}

//! Compare operator
inline bool operator <(const char v, const PIChar & c) {return (PIChar(v) < c);}

//! Compare operator
inline bool operator >=(const char v, const PIChar & c) {return (PIChar(v) >= c);}

//! Compare operator
inline bool operator <=(const char v, const PIChar & c) {return (PIChar(v) <= c);}


//! Compare operator
inline bool operator ==(const char * v, const PIChar & c) {return (PIChar(v) == c);}

//! Compare operator
inline bool operator >(const char * v, const PIChar & c) {return (PIChar(v) > c);}

//! Compare operator
inline bool operator <(const char * v, const PIChar & c) {return (PIChar(v) < c);}

//! Compare operator
inline bool operator >=(const char * v, const PIChar & c) {return (PIChar(v) >= c);}

//! Compare operator
inline bool operator <=(const char * v, const PIChar & c) {return (PIChar(v) <= c);}


//! Compare operator
inline bool operator ==(const int v, const PIChar & c) {return (PIChar(v) == c);}

//! Compare operator
inline bool operator >(const int v, const PIChar & c) {return (PIChar(v) > c);}

//! Compare operator
inline bool operator <(const int v, const PIChar & c) {return (PIChar(v) < c);}

//! Compare operator
inline bool operator >=(const int v, const PIChar & c) {return (PIChar(v) >= c);}

//! Compare operator
inline bool operator <=(const int v, const PIChar & c) {return (PIChar(v) <= c);}

#endif // PICHAR_H
