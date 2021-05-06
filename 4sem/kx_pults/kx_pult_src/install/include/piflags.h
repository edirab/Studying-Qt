/*! \file piflags.h
 * \brief General flags class
*/
/*
    PIP - Platform Independent Primitives
    General flags class
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

#ifndef PIFLAGS_H
#define PIFLAGS_H

#include "pimonitor.h"

/*! \brief This class used as container for bit flags
 * \details PIFlags is wrapper around \c "int". There are many
 * bit-wise operators, native conversion to int and function
 * to test flag. \n Example:
 * \snippet piincludes.cpp flags
 */
template<typename Enum>
class PIP_EXPORT PIFlags {
public:
	//! Constructor with flags = 0
	PIFlags(): flags(0) {;}
	//! Constructor with flags = Enum "e"
	PIFlags(Enum e): flags(e) {;}
	//! Constructor with flags = PIFlags "f"
	PIFlags(const PIFlags & f): flags(f.flags) {;}
	//! Constructor with flags = int "i"
	PIFlags(const int i): flags(i) {;}
	//! Set flags "f" to value "on"
	PIFlags & setFlag(const PIFlags & f, bool on = true) {if (on) flags |= f.flags; else flags &= ~f.flags; return *this;}
	//! Set flag "e" to value "on"
	PIFlags & setFlag(const Enum & e, bool on = true) {if (on) flags |= e; else flags &= ~e; return *this;}
	//! Set flag "i" to value "on"
	PIFlags & setFlag(const int & i, bool on = true) {if (on) flags |= i; else flags &= ~i; return *this;}
	//! copy operator
	void operator =(const PIFlags & f) {flags = f.flags;}
	//! copy operator
	void operator =(const Enum & e) {flags = e;}
	//! copy operator
	void operator =(const int & i) {flags = i;}
	//! compare operator
	bool operator ==(const PIFlags & f) {return flags == f.flags;}
	//! compare operator
	bool operator ==(const Enum & e) {return flags == e;}
	//! compare operator
	bool operator ==(const int i) {return flags == i;}
	//! compare operator
	bool operator !=(const PIFlags & f) {return flags != f.flags;}
	//! compare operator
	bool operator !=(const Enum & e) {return flags != e;}
	//! compare operator
	bool operator !=(const int i) {return flags != i;}
	//! compare operator
	bool operator >(const PIFlags & f) {return flags > f.flags;}
	//! compare operator
	bool operator >(const Enum & e) {return flags > e;}
	//! compare operator
	bool operator >(const int i) {return flags > i;}
	//! compare operator
	bool operator <(const PIFlags & f) {return flags < f.flags;}
	//! compare operator
	bool operator <(const Enum & e) {return flags < e;}
	//! compare operator
	bool operator <(const int i) {return flags < i;}
	//! compare operator
	bool operator >=(const PIFlags & f) {return flags >= f.flags;}
	//! compare operator
	bool operator >=(const Enum & e) {return flags >= e;}
	//! compare operator
	bool operator >=(const int i) {return flags >= i;}
	//! compare operator
	bool operator <=(const PIFlags & f) {return flags <= f.flags;}
	//! compare operator
	bool operator <=(const Enum & e) {return flags <= e;}
	//! compare operator
	bool operator <=(const int i) {return flags <= i;}
	//! Bit-wise AND operator
	void operator &=(const PIFlags & f) {flags &= f.flags;}
	//! Bit-wise AND operator
	void operator &=(const Enum & e) {flags &= e;}
	//! Bit-wise AND operator
	void operator &=(const int i) {flags &= i;}
	//! Bit-wise OR operator
	void operator |=(const PIFlags & f) {flags |= f.flags;}
	//! Bit-wise OR operator
	void operator |=(const Enum & e) {flags |= e;}
	//! Bit-wise OR operator
	void operator |=(const int i) {flags |= i;}
	//! Bit-wise XOR operator
	void operator ^=(const PIFlags & f) {flags ^= f.flags;}
	//! Bit-wise XOR operator
	void operator ^=(const Enum & e) {flags ^= e;}
	//! Bit-wise XOR operator
	void operator ^=(const int i) {flags ^= i;}
	//! Bit-wise AND operator
	PIFlags operator &(PIFlags f) const {PIFlags tf(flags & f.flags); return tf;}
	//! Bit-wise AND operator
	PIFlags operator &(Enum e) const {PIFlags tf(flags & e); return tf;}
	//! Bit-wise AND operator
	PIFlags operator &(int i) const {PIFlags tf(flags & i); return tf;}
	//! Bit-wise OR operator
	PIFlags operator |(PIFlags f) const {PIFlags tf(flags | f.flags); return tf;}
	//! Bit-wise OR operator
	PIFlags operator |(Enum e) const {PIFlags tf(flags | e); return tf;}
	//! Bit-wise OR operator
	PIFlags operator |(int i) const {PIFlags tf(flags | i); return tf;}
	//! Bit-wise XOR operator
	PIFlags operator ^(PIFlags f) const {PIFlags tf(flags ^ f.flags); return tf;}
	//! Bit-wise XOR operator
	PIFlags operator ^(Enum e) const {PIFlags tf(flags ^ e); return tf;}
	//! Bit-wise XOR operator
	PIFlags operator ^(int i) const {PIFlags tf(flags ^ i); return tf;}
	//! Test flag operator
	bool operator [](Enum e) const {return (flags & e) == e;}
	//! Implicity conversion to \c int
	operator int() const {return flags;}
private:
	int flags;
};

#endif // PIFLAGS_H
