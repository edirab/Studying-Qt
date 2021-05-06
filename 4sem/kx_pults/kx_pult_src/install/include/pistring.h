/*! \file pistring.h
 * \brief String
 * 
 * This file declare string and string list classes
*/
/*
    PIP - Platform Independent Primitives
    String
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

#ifndef PISTRING_H
#define PISTRING_H

#include "pibytearray.h"
#include "pichar.h"

#define PIStringAscii PIString::fromAscii

class PIStringList;

class PIP_EXPORT PIString: public PIDeque<PIChar>
{
	friend PIByteArray & operator >>(PIByteArray & s, PIString & v);
public:
	//! Contructs an empty string
	PIString(): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--;}

	//inline PIString & operator +=(const char c) {push_back(c); return *this;}
	PIString & operator +=(const PIChar & c) {push_back(c); return *this;}
	PIString & operator +=(const char * str);
	PIString & operator +=(const wchar_t * str);
	PIString & operator +=(const std::string & str) {appendFromChars(str.c_str(), str.length()); return *this;}
	PIString & operator +=(const PIByteArray & ba) {appendFromChars((const char * )ba.data(), ba.size_s()); return *this;}
	PIString & operator +=(const PIString & str);
#ifdef HAS_LOCALE
	PIString & operator +=(const wstring & str);
#endif

	//PIString(const char c) {*this += c;}
	PIString(const PIString & o): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this += o;}

	
	//! Contructs string with single symbol "c"
	PIString(const PIChar & c): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this += c;}
	PIString(const char c): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this += PIChar(c);}
	
	/*! \brief Contructs string from c-string "str"
	 * \details "str" should be null-terminated\n
	 * Example: \snippet pistring.cpp PIString(char * ) */
	PIString(const char * str): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this += str;}
	
	/*! \brief Contructs string from \c wchar_t c-string "str"
	 * \details "str" should be null-terminated\n
	 * Example: \snippet pistring.cpp PIString(wchar_t * ) */
	PIString(const wchar_t * str): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this += str;}
	
	//! Contructs string from std::string "str"
	PIString(const std::string & str): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this += str;}

#ifdef HAS_LOCALE
	PIString(const wstring & str): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this += str;}
#endif

	//! Contructs string from byte array "ba"
	PIString(const PIByteArray & ba): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this += ba;}

	//! \brief Contructs string from "len" characters of buffer "str"
	PIString(const PIChar * str, const int len): PIDeque<PIChar>(str, size_t(len)) {/*reserve(256); */piMonitor.strings++; piMonitor.containers--;}

	/*! \brief Contructs string from "len" characters of buffer "str"
	 * \details Example: \snippet pistring.cpp PIString(char * , int) */
	PIString(const char * str, const int len): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this += std::string(str, len);}

	/*! \brief Contructs string as sequence of characters "c" of buffer with length "len"
	 * \details Example: \snippet pistring.cpp PIString(int, char) */
	PIString(const int len, const char c): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; for (int i = 0; i < len; ++i) push_back(c);}

	/*! \brief Contructs string as sequence of symbols "c" of buffer with length "len"
	 * \details Example: \snippet pistring.cpp PIString(int, PIChar) */
	PIString(const int len, const PIChar & c): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; for (int i = 0; i < len; ++i) push_back(c);}
/*
#ifdef WINDOWS
	PIString(const WCHAR * str): PIDeque<PIChar>() {piMonitor.strings++; piMonitor.containers--; *this += str;}
	PIString & operator +=(const WCHAR * str);
	PIString & operator <<(const WCHAR * str) {*this += str; return *this;}
#endif
*/
	
	PIString(const short & value): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this = fromNumber(value);}
	PIString(const ushort & value): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this = fromNumber(value);}
	PIString(const int & value): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this = fromNumber(value);}
	PIString(const uint & value): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this = fromNumber(value);}
	PIString(const long & value): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this = fromNumber(value);}
	PIString(const ulong & value): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this = fromNumber(value);}
	PIString(const llong & value): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this = fromNumber(value);}
	PIString(const ullong & value): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this = fromNumber(value);}
	PIString(const float & value): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this = fromNumber(value);}
	PIString(const double & value): PIDeque<PIChar>() {/*reserve(256); */piMonitor.strings++; piMonitor.containers--; *this = fromNumber(value);}
	
	
	//~PIString() {piMonitor.strings--; piMonitor.containers++;}


	PIString & operator =(const PIString & o) {if (this == &o) return *this; clear(); *this += o; return *this;}

	/*! \brief Return c-string representation of string
	 * \details Converts content of string to c-string and return
	 * pointer to first char. This buffer is valid until new convertion
	 * or execution \a data() or \a toByteArray().\n
	 * Example: \snippet pistring.cpp PIString::char* */
	operator const char*() {return data();}

	//! Return std::string representation of string
	//operator const std::string() {if (size() == 0) return std::string(); std::string s; for (int i = 0; i < length(); ++i) s.push_back(at(i).toAscii()); return s;}
	
	//! Return symbol at index "pos"
	PIChar operator [](const int pos) const {return at(pos);}
	
	//! Return reference to symbol at index "pos"
	PIChar & operator [](const int pos) {return at(pos);}

	//! Compare operator
	bool operator ==(const PIString & str) const;

	//! Compare operator
	bool operator ==(const PIChar c) const {return *this == PIString(c);}
	//inline bool operator ==(const char c) const {return *this == PIString(c);}

	//! Compare operator
	bool operator ==(const char * str) const {return *this == PIString(str);}

	//! Compare operator
	bool operator ==(const std::string & str) const {return *this == PIString(str);}


	//! Compare operator
	bool operator !=(const PIString & str) const;

	//! Compare operator
	bool operator !=(const PIChar c) const {return *this != PIString(c);}
	//inline bool operator !=(const char c) const {return *this != PIString(c);}

	//! Compare operator
	bool operator !=(const char * str) const {return *this != PIString(str);}

	//! Compare operator
	bool operator !=(const std::string & str) const {return *this != PIString(str);}


	//! Compare operator
	bool operator <(const PIString & str) const;

	//! Compare operator
	bool operator <(const PIChar c) const {return *this < PIString(c);}
	//inline bool operator <(const char c) const {return *this < PIString(c);}

	//! Compare operator
	bool operator <(const char * str) const {return *this < PIString(str);}

	//! Compare operator
	bool operator <(const std::string & str) const {return *this < PIString(str);}


	//! Compare operator
	bool operator >(const PIString & str) const;

	//! Compare operator
	bool operator >(const PIChar c) const {return *this > PIString(c);}
	//inline bool operator >(const char c) const {return *this > PIString(c);}

	//! Compare operator
	bool operator >(const char * str) const {return *this > PIString(str);}

	//! Compare operator
	bool operator >(const std::string & str) const {return *this > PIString(str);}


	//! Compare operator
	bool operator <=(const PIString & str) const {return !(*this > str);}

	//! Compare operator
	bool operator <=(const PIChar c) const {return *this <= PIString(c);}
	//inline bool operator <=(const char c) const {return *this <= PIString(c);}

	//! Compare operator
	bool operator <=(const char * str) const {return *this <= PIString(str);}

	//! Compare operator
	bool operator <=(const std::string & str) const {return *this <= PIString(str);}


	//! Compare operator
	bool operator >=(const PIString & str) const {return !(*this < str);}

	//! Compare operator
	bool operator >=(const PIChar c) const {return *this >= PIString(c);}
	//inline bool operator >=(const char c) const {return *this >= PIString(c);}

	//! Compare operator
	bool operator >=(const char * str) const {return *this >= PIString(str);}

	//! Compare operator
	bool operator >=(const std::string & str) const {return *this >= PIString(str);}


	operator bool() const {return toBool();}
	operator short() const {return toShort();}
	operator ushort() const {return toUShort();}
	operator int() const {return toInt();}
	operator uint() const {return toUInt();}
	operator long() const {return toLong();}
	operator ulong() const {return toULong();}
	operator llong() const {return toLLong();}
	operator ullong() const {return toULLong();}
	operator float() const {return toFloat();}
	operator double() const {return toDouble();}


	/*! \brief Append string "str" at the end of string
	 * \details Example: \snippet pistring.cpp PIString::<<(PIString) */
	PIString & operator <<(const PIString & str) {*this += str; return *this;}
	//inline PIString & operator <<(const char c) {*this += c; return *this;}

	/*! \brief Append symbol "c" at the end of string
	 * \details Example: \snippet pistring.cpp PIString::<<(PIChar) */
	PIString & operator <<(const PIChar & c) {*this += c; return *this;}

	/*! \brief Append c-string "str" at the end of string
	 * \details Example: \snippet pistring.cpp PIString::<<(char * ) */
	PIString & operator <<(const char * str) {*this += str; return *this;}

	/*! \brief Append \c wchar_t c-string "str" at the end of string
	 * \details Example: \snippet pistring.cpp PIString::<<(wchar_t * ) */
	PIString & operator <<(const wchar_t * str) {*this += str; return *this;}
	
	//! Append std::string "str" at the end of string
	PIString & operator <<(const std::string & str) {*this += str; return *this;}

	/*! \brief Append string representation of "num" at the end of string
	 * \details Example: \snippet pistring.cpp PIString::<<(int) */
	PIString & operator <<(const int & num) {*this += PIString::fromNumber(num); return *this;}
	PIString & operator <<(const uint & num) {*this += PIString::fromNumber(num); return *this;}
	
	/*! \brief Append string representation of "num" at the end of string
	 * \details Example: \snippet pistring.cpp PIString::<<(int) */
	PIString & operator <<(const short & num) {*this += PIString::fromNumber(num); return *this;}
	PIString & operator <<(const ushort & num) {*this += PIString::fromNumber(num); return *this;}
	
	/*! \brief Append string representation of "num" at the end of string
	 * \details Example: \snippet pistring.cpp PIString::<<(int) */
	PIString & operator <<(const long & num) {*this += PIString::fromNumber(num); return *this;}
	PIString & operator <<(const ulong & num) {*this += PIString::fromNumber(num); return *this;}
	
	PIString & operator <<(const llong & num) {*this += PIString::fromNumber(num); return *this;}
	PIString & operator <<(const ullong & num) {*this += PIString::fromNumber(num); return *this;}
	
	/*! \brief Append string representation of "num" at the end of string
	 * \details Example: \snippet pistring.cpp PIString::<<(int) */
	PIString & operator <<(const float & num) {*this += PIString::fromNumber(num); return *this;}
	
	/*! \brief Append string representation of "num" at the end of string
	 * \details Example: \snippet pistring.cpp PIString::<<(int) */
	PIString & operator <<(const double & num) {*this += PIString::fromNumber(num); return *this;}
	
	
	//! \brief Insert string "str" at the begin of string
	PIString & prepend(const PIString & str) {insert(0, str); return *this;}
	
	//! \brief Insert string "str" at the end of string
	PIString & append(const PIString & str) {*this += str; return *this;}

	
	/*! \brief Return part of string from symbol at index "start" and maximum length "len"
	 * \details All variants demonstrated in example: \snippet pistring.cpp PIString::mid
	 * \sa \a left(), \a right() */
	PIString mid(const int start, const int len = -1) const;
	
	/*! \brief Return part of string from left and maximum length "len"
	 * \details Example: \snippet pistring.cpp PIString::left
	 * \sa \a mid(), \a right() */
	PIString left(const int len) const {return len <= 0 ? PIString() : mid(0, len);}
	
	/*! \brief Return part of string from right and maximum length "len"
	 * \details Example: \snippet pistring.cpp PIString::right
	 * \sa \a mid(), \a left() */
	PIString right(const int len) const {return len <= 0 ? PIString() : mid(size() - len, len);}
	
	/*! \brief Remove part of string from symbol as index "start" and maximum length "len"
	 * and return this string
	 * \details All variants demonstrated in example: \snippet pistring.cpp PIString::cutMid
	 * \sa \a cutLeft(), \a cutRight() */
	PIString & cutMid(const int start, const int len);
	
	/*! \brief Remove part of string from left and maximum length "len" and return this string
	 * \details Example: \snippet pistring.cpp PIString::cutLeft
	 * \sa \a cutMid(), \a cutRight() */
	PIString & cutLeft(const int len) {return len <= 0 ? *this : cutMid(0, len);}
	
	/*! \brief Remove part of string from right and maximum length "len" and return this string
	 * \details Example: \snippet pistring.cpp PIString::cutRight
	 * \sa \a cutMid(), \a cutLeft() */
	PIString & cutRight(const int len) {return len <= 0 ? *this : cutMid(size() - len, len);}
	
	/*! \brief Remove spaces at the start and at the end of string and return this string
	 * \details Example: \snippet pistring.cpp PIString::trim
	 * \sa \a trimmed() */
	PIString & trim();
	
	/*! \brief Return copy of this string without spaces at the start and at the end
	 * \details Example: \snippet pistring.cpp PIString::trimmed
	 * \sa \a trim() */
	PIString   trimmed() const;
	
	/*! \brief Replace part of string from index "from" and maximum length "len"
	 * with string "with" and return this string
	 * \details Example: \snippet pistring.cpp PIString::replace_0
	 * \sa \a replaced(), \a replaceAll() */
	PIString & replace(const int from, const int count, const PIString & with);
	
	/*! \brief Replace part copy of this string from index "from" and maximum length "len"
	 * with string "with" and return copied string
	 * \details Example: \snippet pistring.cpp PIString::replaced_0
	 * \sa \a replace(), \a replaceAll() */
	PIString   replaced(const int from, const int count, const PIString & with) const {PIString str(*this); str.replace(from, count, with); return str;}
	
	/*! \brief Replace first founded substring "what" with string "with" and return this string
	 * \details If "ok" is not null, it set to "true" if something was replaced\n
	 * Example: \snippet pistring.cpp PIString::replace_1
	 * \sa \a replaced(), \a replaceAll() */
	PIString & replace(const PIString & what, const PIString & with, bool * ok = 0);
	
	/*! \brief Replace first founded substring "what" with string "with" and return copied string
	 * \details If "ok" is not null, it set to "true" if something was replaced\n
	 * Example: \snippet pistring.cpp PIString::replaced_1
	 * \sa \a replaced(), \a replaceAll() */
	PIString   replaced(const PIString & what, const PIString & with, bool * ok = 0) const {PIString str(*this); str.replace(what, with, ok); return str;}
	
	/*! \brief Replace all founded substrings "what" with strings "with" and return this string
	 * \details Example: \snippet pistring.cpp PIString::replaceAll
	 * \sa \a replace(), \a replaced() */
	PIString & replaceAll(const PIString & what, const PIString & with);
	PIString   replaceAll(const PIString & what, const PIString & with) const {PIString str(*this); str.replaceAll(what, with); return str;}
	
	/*! \brief Repeat content of string "times" times and return this string
	 * \details Example: \snippet pistring.cpp PIString::repeat */
	PIString & repeat(int times) {PIString ss(*this); times--; piForTimes (times) *this += ss; return *this;}
	
	/*! \brief Returns repeated "times" times string
	 * \details Example: \snippet pistring.cpp PIString::repeated */
	PIString repeated(int times) const {PIString ss(*this); return ss.repeat(times);}
	
	/*! \brief Insert symbol "c" after index "index" and return this string
	 * \details Example: \snippet pistring.cpp PIString::insert_0 */
	PIString & insert(const int index, const PIChar & c) {PIDeque<PIChar>::insert(index, c); return *this;}
	
	/*! \brief Insert symbol "c" after index "index" and return this string
	 * \details Example: \snippet pistring.cpp PIString::insert_1 */
	PIString & insert(const int index, const char & c) {return insert(index, PIChar(c));}
	
	/*! \brief Insert string "str" after index "index" and return this string
	 * \details Example: \snippet pistring.cpp PIString::insert_2 */
	PIString & insert(const int index, const PIString & str);
	
	/*! \brief Insert string "str" after index "index" and return this string
	 * \details Example: \snippet pistring.cpp PIString::insert_2 */
	PIString & insert(const int index, const char * c) {return insert(index, PIString(c));}
	
	/*! \brief Enlarge string to length "len" by addition sequence of symbols
	 * "c" at the end of string, and return this string
	 * \details Example: \snippet pistring.cpp PIString::expandRightTo
	 * \sa \a expandLeftTo() */
	PIString & expandRightTo(const int len, const PIChar & c) {if (len > length()) resize(len, c); return *this;}
	
	/*! \brief Enlarge string to length "len" by addition sequence of symbols
	 * "c" at the beginning of string, and return this string
	 * \details Example: \snippet pistring.cpp PIString::expandLeftTo
	 * \sa \a expandRightTo() */
	PIString & expandLeftTo(const int len, const PIChar & c) {if (len > length()) insert(0, PIString(len - length(), c)); return *this;}
	
	/*! \brief Reverse string and return this string
	 * \details Example: \snippet pistring.cpp PIString::reverse 
	 * \sa \a reversed() */
	PIString & reverse() {PIString str(*this); clear(); piForeachR (const PIChar & c, str) push_back(c); return *this;}
	
	/*! \brief Reverse copy of this string and return it
	 * \details Example: \snippet pistring.cpp PIString::reversed
	 * \sa \a reverse() */
	PIString   reversed() const {PIString str(*this); str.reverse(); return str;}
	
	
	/*! \brief Take a part of string from symbol at index "start" and maximum length "len" and return it
	 * \details Example: \snippet pistring.cpp PIString::takeMid
	 * \sa \a takeLeft, \a takeRight() */
	PIString takeMid(const int start, const int len = -1) {PIString ret(mid(start, len)); cutMid(start, len); return ret;}
	
	/*! \brief Take a part from the begin of string with maximum length "len" and return it
	 * \details Example: \snippet pistring.cpp PIString::takeLeft
	 * \sa \a takeMid(), \a takeRight() */
	PIString takeLeft(const int len) {PIString ret(left(len)); cutLeft(len); return ret;}
	
	/*! \brief Take a part from the end of string with maximum length "len" and return it
	 * \details Example: \snippet pistring.cpp PIString::takeRight
	 * \sa \a takeMid(), \a takeLeft() */
	PIString takeRight(const int len) {PIString ret(right(len)); cutRight(len); return ret;}
	
	/*! \brief Take a symbol from the begin of this string and return it
	 * \details Example: \snippet pistring.cpp PIString::takeSymbol
	 * \sa \a takeWord(), \a takeCWord(), \a takeLine(), \a takeNumber(), \a takeRange() */
	PIString takeSymbol();
	
	/*! \brief Take a word from the begin of this string and return it
	 * \details Example: \snippet pistring.cpp PIString::takeWord
	 * \sa \a takeSymbol(), \a takeCWord(), \a takeLine(), \a takeNumber(), \a takeRange() */
	PIString takeWord();
	
	/*! \brief Take a word with letters, numbers and '_' symbols from the
	 * begin of this string and return it
	 * \details Example: \snippet pistring.cpp PIString::takeCWord
	 * \sa \a takeSymbol(), \a takeWord(), \a takeLine(), \a takeNumber(), \a takeRange() */
	PIString takeCWord();
	
	/*! \brief Take a line from the begin of this string and return it
	 * \details Example: \snippet pistring.cpp PIString::takeLine
	 * \sa \a takeSymbol(), \a takeWord(), \a takeCWord(), \a takeNumber(), \a takeRange() */
	PIString takeLine();
	
	/*! \brief Take a number with C-format from the begin of this string and return it
	 * \details Example: \snippet pistring.cpp PIString::takeNumber
	 * \sa \a takeSymbol(), \a takeWord(), \a takeCWord(), \a takeLine(), \a takeRange() */
	PIString takeNumber();
	
	/*! \brief Take a range between "start" and "end" symbols from the begin of this
	 * string and return it.
	 * \details "Shield" symbol prevent analysis of the next symbol.
	 * Example: \snippet pistring.cpp PIString::takeRange
	 * \sa \a takeSymbol(), \a takeWord(), \a takeLine(), \a takeNumber() */
	PIString takeRange(const PIChar & start, const PIChar & end, const PIChar & shield = '\\');


	/*! \brief Return a string in brackets "start" and "end" symbols from the begin of this
	 * string and return it.
	 * \details Example: string = "a(b(c)d)e"; inBrackets('(', ')') = "b(c)d"; */
	PIString inBrackets(const PIChar & start, const PIChar & end) const;
	
	/*! \brief Return real bytes count of this string
	 * \details It`s equivalent length of char sequence
	 * returned by function \a data() - 1, without terminating null-char \n
	 * Example: \snippet pistring.cpp PIString::lengthAscii
	 * \sa \a data() */
	int lengthAscii() const {buildData(); return data_.size_s() - 1;}
	
	/*! \brief Return \c char * representation of this string in system codepage
	 * \details This function fill buffer by sequence
	 * of chars. Minimum length of this buffer is count
	 * of symbols. Returned \c char * is valid until next
	 * execution of this function.\n
	 * Example: \snippet pistring.cpp PIString::data
	 * \sa \a dataConsole(), \a dataUTF8() */
	const char * data() const {buildData(); return (const char *)(data_.data());}
	
	/*! \brief Return \c char * representation of this string in terminal codepage
	 * \details This function fill buffer by sequence
	 * of chars. Minimum length of this buffer is count
	 * of symbols. Returned \c char * is valid until next
	 * execution of this function.\n
	 * \sa \a data(), \a dataUTF8() */
	const char * dataConsole() const;
	
	/*! \brief Return \c char * representation of this string in UTF-8
	 * \details This function fill buffer by sequence
	 * of chars. Minimum length of this buffer is count
	 * of symbols. Returned \c char * is valid until next
	 * execution of this function.\n
	 * \sa \a data(), \a dataConsole() */
	const char * dataUTF8() const;
	
	/*! \brief Return \c char * representation of this string in ASCII
	 * \details This function fill buffer by sequence
	 * of chars. Minimum length of this buffer is count
	 * of symbols. Returned \c char * is valid until next
	 * execution of this function.\n */
	const char * dataAscii() const;

	//! \brief Return \c std::string representation of this string
	std::string stdString() const {return convertToStd();}
	
#ifdef HAS_LOCALE
	wstring stdWString() const {return convertToWString();}
#endif

	//! \brief Return \a PIByteArray contains \a data() of this string without terminating null-char
	PIByteArray toByteArray() const {buildData(); return data_.resized(data_.size_s() - 1);}

	//! \brief Return \a PIByteArray contains UTF-8 \a data() of this string without terminating null-char
	PIByteArray toUTF8() const;

	//! \brief Return \a PIByteArray contains custom charset representation of this string without terminating null-char
	PIByteArray toCharset(const char * c) const;

	/*! \brief Split string with delimiter "delim" to \a PIStringList and return it
	 * \details Example: \snippet pistring.cpp PIString::split */
	PIStringList split(const PIString & delim) const;


	//! \brief Convert each symbol in copyed string to upper case and return it
	PIString toUpperCase() const;
	
	//! \brief Convert each symbol in copyed string to lower case and return it
	PIString toLowerCase() const;
	
	
	PIString toNativeDecimalPoints() const {
#ifdef HAS_LOCALE
		PIString s(*this); if (currentLocale == 0) return s; return s.replaceAll(".", currentLocale->decimal_point).replaceAll(",", currentLocale->decimal_point);
#else
		return PIString(*this).replaceAll(",", ".");
#endif
	}

	
	//! \brief Search substring "str" from symbol at index "start" and return first occur position
	//! \details Example: \snippet pistring.cpp PIString::find
	int find(const char str, const int start = 0) const;
	
	//! \brief Search substring "str" from symbol at index "start" and return first occur position
	//! \details Example: \snippet pistring.cpp PIString::find
	int find(const PIString & str, const int start = 0) const;
	
	//! \brief Search substring "str" from symbol at index "start" and return first occur position
	//! \details Example: \snippet pistring.cpp PIString::find
	int find(const char * str, const int start = 0) const {return find(PIString(str), start);}
	
	//! \brief Search substring "str" from symbol at index "start" and return last occur position
	//! \details Example: \snippet pistring.cpp PIString::findLast
	int findLast(const char str, const int start = 0) const;
	
	//! \brief Search substring "str" from symbol at index "start" and return last occur position
	//! \details Example: \snippet pistring.cpp PIString::findLast
	int findLast(const PIString & str, const int start = 0) const;
	
	//! \brief Search substring "str" from symbol at index "start" and return last occur position
	//! \details Example: \snippet pistring.cpp PIString::findLast
	int findLast(const char * str, const int start = 0) const {return findLast(PIString(str), start);}
	
	//! \brief Search word "word" from symbol at index "start" and return first occur position.
	//! \details Example: \snippet pistring.cpp PIString::findWord
	int findWord(const PIString & word, const int start = 0) const;
	
	//! \brief Search C-style word "word" from symbol at index "start" and return first occur position.
	//! \details Example: \snippet pistring.cpp PIString::findCWord
	int findCWord(const PIString & word, const int start = 0) const;
	
	//! \brief Return if string starts with "str"
	bool startsWith(const PIString & str) const;
	
	//! \brief Return if string ends with "str"
	bool endsWith(const PIString & str) const;
	
	//! \brief Return symbols length of string
	int length() const {return size();}
	
	//! \brief Return \c true if string is empty, i.e. length = 0
	bool isEmpty() const {return (size() == 0 || *this == "");}

	
	//! \brief Return \c true if string equal "true", "yes", "on" or positive not null numeric value
	bool toBool() const;
	
	//! \brief Return \c char numeric value of string
	char toChar() const;
	
	//! \brief Return \c short numeric value of string in base "base"
	//! \details Example: \snippet pistring.cpp PIString::toNumber
	short toShort(int base = -1, bool * ok = 0) const {return short(toNumberBase(*this, base, ok));}
	
	//! \brief Return \c ushort numeric value of string in base "base"
	//! \details Example: \snippet pistring.cpp PIString::toNumber
	ushort toUShort(int base = -1, bool * ok = 0) const {return ushort(toNumberBase(*this, base, ok));}
	
	//! \brief Return \c int numeric value of string in base "base"
	//! \details Example: \snippet pistring.cpp PIString::toNumber
	int toInt(int base = -1, bool * ok = 0) const {return int(toNumberBase(*this, base, ok));}
	
	//! \brief Return \c uint numeric value of string in base "base"
	//! \details Example: \snippet pistring.cpp PIString::toNumber
	uint toUInt(int base = -1, bool * ok = 0) const {return uint(toNumberBase(*this, base, ok));}
	
	//! \brief Return \c long numeric value of string in base "base"
	//! \details Example: \snippet pistring.cpp PIString::toNumber
	long toLong(int base = -1, bool * ok = 0) const {return long(toNumberBase(*this, base, ok));}
	
	//! \brief Return \c ulong numeric value of string in base "base"
	//! \details Example: \snippet pistring.cpp PIString::toNumber
	ulong toULong(int base = -1, bool * ok = 0) const {return ulong(toNumberBase(*this, base, ok));}
	
	//! \brief Return \c llong numeric value of string in base "base"
	//! \details Example: \snippet pistring.cpp PIString::toNumber
	llong toLLong(int base = -1, bool * ok = 0) const {return toNumberBase(*this, base, ok);}
	
	//! \brief Return \c ullong numeric value of string in base "base"
	//! \details Example: \snippet pistring.cpp PIString::toNumber
	ullong toULLong(int base = -1, bool * ok = 0) const {return ullong(toNumberBase(*this, base, ok));}
	
	//! \brief Return \c float numeric value of string
	//! \details Example: \snippet pistring.cpp PIString::toFloat
	float toFloat() const {return (float)atof(toNativeDecimalPoints().data());}
	
	//! \brief Return \c double numeric value of string
	//! \details Example: \snippet pistring.cpp PIString::toFloat
	double toDouble() const {return atof(toNativeDecimalPoints().data());}
	
	//! \brief Return \c ldouble numeric value of string
	//! \details Example: \snippet pistring.cpp PIString::toFloat
	ldouble toLDouble() const {return atof(toNativeDecimalPoints().data());}

	//inline PIString & setNumber(const char value) {clear(); *this += itos(value); return *this;}
	
	//! \brief Set string content to numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::setNumber
	PIString & setNumber(const short value, int base = 10, bool * ok = 0) {clear(); *this += PIString::fromNumber(value, base, ok); return *this;}
	
	//! \brief Set string content to numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::setNumber
	PIString & setNumber(const ushort value, int base = 10, bool * ok = 0) {clear(); *this += PIString::fromNumber(value, base, ok); return *this;}
	
	//! \brief Set string content to numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::setNumber
	PIString & setNumber(const int value, int base = 10, bool * ok = 0) {clear(); *this += PIString::fromNumber(value, base, ok); return *this;}
	
	//! \brief Set string content to numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::setNumber
	PIString & setNumber(const uint value, int base = 10, bool * ok = 0) {clear(); *this += PIString::fromNumber(value, base, ok); return *this;}
	
	//! \brief Set string content to numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::setNumber
	PIString & setNumber(const long value, int base = 10, bool * ok = 0) {clear(); *this += PIString::fromNumber(value, base, ok); return *this;}
	
	//! \brief Set string content to numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::setNumber
	PIString & setNumber(const ulong value, int base = 10, bool * ok = 0) {clear(); *this += PIString::fromNumber(value, base, ok); return *this;}
	
	//! \brief Set string content to numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::setNumber
	PIString & setNumber(const llong & value, int base = 10, bool * ok = 0) {clear(); *this += PIString::fromNumber(value, base, ok); return *this;}
	
	//! \brief Set string content to numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::setNumber
	PIString & setNumber(const ullong & value, int base = 10, bool * ok = 0) {clear(); *this += PIString::fromNumber(value, base, ok); return *this;}
	
	//! \brief Set string content to numeric representation of "value"
	//! \details Example: \snippet pistring.cpp PIString::setFloat
	PIString & setNumber(const float value) {clear(); *this += PIString::fromNumber(value); return *this;}
	
	//! \brief Set string content to numeric representation of "value"
	//! \details Example: \snippet pistring.cpp PIString::setFloat
	PIString & setNumber(const double & value) {clear(); *this += PIString::fromNumber(value); return *this;}
	
	//! \brief Set string content to numeric representation of "value"
	//! \details Example: \snippet pistring.cpp PIString::setFloat
	PIString & setNumber(const ldouble & value) {clear(); *this += PIString::fromNumber(value); return *this;}
	
	//! \brief Set string content to human readable size in B/kB/MB/GB/TB
	//! \details Example: \snippet pistring.cpp PIString::setReadableSize
	PIString & setReadableSize(llong bytes);

	//inline static PIString fromNumber(const char value) {return PIString(itos(value));}
	
	//! \brief Return string contains numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::fromNumber
	static PIString fromNumber(const short value, int base = 10, bool * ok = 0) {return fromNumberBaseS(llong(value), base, ok);}
	
	//! \brief Return string contains numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::fromNumber
	static PIString fromNumber(const ushort value, int base = 10, bool * ok = 0) {return fromNumberBaseU(ullong(value), base, ok);}
	
	//! \brief Return string contains numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::fromNumber
	static PIString fromNumber(const int value, int base = 10, bool * ok = 0) {return fromNumberBaseS(llong(value), base, ok);}
	
	//! \brief Return string contains numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::fromNumber
	static PIString fromNumber(const uint value, int base = 10, bool * ok = 0) {return fromNumberBaseU(ullong(value), base, ok);}
	
	//! \brief Return string contains numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::fromNumber
	static PIString fromNumber(const long value, int base = 10, bool * ok = 0) {return fromNumberBaseS(llong(value), base, ok);}
	
	//! \brief Return string contains numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::fromNumber
	static PIString fromNumber(const ulong value, int base = 10, bool * ok = 0) {return fromNumberBaseU(ullong(value), base, ok);}
	
	//! \brief Return string contains numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::fromNumber
	static PIString fromNumber(const llong & value, int base = 10, bool * ok = 0) {return fromNumberBaseS(value, base, ok);}
	
	//! \brief Return string contains numeric representation of "value" in base "base"
	//! \details Example: \snippet pistring.cpp PIString::fromNumber
	static PIString fromNumber(const ullong & value, int base = 10, bool * ok = 0) {return fromNumberBaseU(value, base, ok);}
	
	//! \brief Return string contains numeric representation of "value"
	//! \details Example: \snippet pistring.cpp PIString::fromFloat
	static PIString fromNumber(const float value) {return ftos(value);}
	
	//! \brief Return string contains numeric representation of "value"
	//! \details Example: \snippet pistring.cpp PIString::fromFloat
	static PIString fromNumber(const double & value) {return ftos(value);}
	
	//! \brief Return string contains numeric representation of "value"
	//! \details Example: \snippet pistring.cpp PIString::fromFloat
	static PIString fromNumber(const ldouble & value) {return ftos(value);}
	
	//! \brief Return "true" or "false"
	static PIString fromBool(const bool value) {return PIString(value ? "true" : "false");}
	
	//! \brief Return string constructed from terminal codepage
	static PIString fromConsole(const char * s);
	
	//! \brief Return string constructed from system codepage
	static PIString fromSystem(const char * s);
	
	//! \brief Return string constructed from UTF-8
	static PIString fromUTF8(const char * s);
	
	//! \brief Return string constructed from ASCII
	static PIString fromAscii(const char * s);

	//! \brief Return string constructed from "c" codepage
	static PIString fromCodepage(const char * s, const char * c);

	//! \brief Return string contains human readable size in B/kB/MB/GB/TB
	//! \details Example: \snippet pistring.cpp PIString::readableSize
	static PIString readableSize(llong bytes) {PIString s; s.setReadableSize(bytes); return s;}

	PIString & removeAll(char v) {replaceAll(v, ""); return *this;}
	PIString & removeAll(const PIString & v) {replaceAll(v, ""); return *this;}

private:
	static const char toBaseN[];
	static const int fromBaseN[];
	
	static PIString itos(const int num);
	static PIString ltos(const long num);
	static PIString uitos(const uint num);
	static PIString ultos(const ulong num);
	static PIString ftos(const float num);
	static PIString dtos(const double num);
	static PIString fromNumberBaseS(const llong value, int base = 10, bool * ok = 0) {
		if (value == 0) return PIString("0");
		if (base < 2 || base > 40) {if (ok != 0) *ok = false; return PIString();}
		if (ok != 0) *ok = true;
		if (base == 10) return itos(value);
		PIString ret;
		llong v = value < 0 ? -value : value, cn;
		int b = base;
		while (v >= llong(base)) {
			cn = v % b;
			v /= b;
			//cout << int(cn) << ", " << int(v) << endl;
			ret.push_front(PIChar(toBaseN[cn]));
		}
		if (v > 0) ret.push_front(PIChar(toBaseN[v]));
		if (value < 0) ret.push_front('-');
		return ret;
	}
	static PIString fromNumberBaseU(const ullong value, int base = 10, bool * ok = 0) {
		if (value == 0) return PIString("0");
		if (base < 2 || base > 40) {if (ok != 0) *ok = false; return PIString();}
		if (ok != 0) *ok = true;
		if (base == 10) return itos(value);
		PIString ret;
		ullong v = value, cn;
		int b = base;
		while (v >= ullong(base)) {
			cn = v % b;
			v /= b;
			//cout << int(cn) << ", " << int(v) << endl;
			ret.push_front(PIChar(toBaseN[cn]));
		}
		if (v > 0) ret.push_front(PIChar(toBaseN[v]));
		return ret;
	}
	static llong toNumberBase(const PIString & value, int base = -1, bool * ok = 0) {
		PIString v = value.trimmed();
		if (base < 0) {
			int ind = v.find("0x");
			if (ind == 0 || ind == 1) {v.remove(ind, 2); base = 16;}
			else base = 10;
		} else
			if (base < 2 || base > 40) {if (ok != 0) *ok = false; return 0;}
		//v.reverse();
		if (ok) *ok = true;
		PIVector<int> digits;
		llong ret = 0, m = 1;
		bool neg = false;
		int cs;
		for (int i = 0; i < v.size_s(); ++i) {
			if (v[i] == PIChar('-')) {neg = !neg; continue;}
			cs = fromBaseN[int(v[i].toAscii())];
			if (cs < 0 || cs >= base) {
				if (ok) *ok = false;
				break;
			}
			digits << cs;
		}
		for (int i = digits.size_s() - 1; i >= 0; --i) {
			ret += digits[i] * m;
			m *= base;
		}
		if (neg) ret = -ret;
		/*piForeachC (PIChar & i, v) {
			if (i == PIChar('-')) {ret = -ret; continue;}
			cs = fromBaseN[int(i.toAscii())];
			cout << i << " = " << cs << endl;
			if (cs < 0 || cs >= base) return ret;
			ret += cs * m;
			m *= base;
		}*/
		return ret;
	}
	void appendFromChars(const char * c, int s, const char * cp = 0);
	void buildData(const char * cp = 0) const;
	void trimsubstr(int &st, int &fn) const;
	std::string convertToStd() const;
#ifdef HAS_LOCALE
	wstring convertToWString() const {wstring s; for (int i = 0; i < length(); ++i) s.push_back(at(i).toWChar()); return s;}
#endif

	mutable PIByteArray data_;
	//string std_string;
	//wstring std_wstring;

};


//! \relatesalso PIString \brief Output operator to std::ostream (cout)
inline std::ostream & operator <<(std::ostream & s, const PIString & v) {for (int i = 0; i < v.length(); ++i) s << v[i]; return s;}

//! \relatesalso PIString \brief Input operator from std::istream (cin)
inline std::istream & operator >>(std::istream & s, PIString & v) {std::string ss; s >> ss; v << PIString(ss); return s;}

//! \relatesalso PIString \relatesalso PICout \brief Output operator to PICout
PICout operator <<(PICout s, const PIString & v);


//! \relatesalso PIString \relatesalso PIByteArray \brief Output operator to PIByteArray
inline PIByteArray & operator <<(PIByteArray & s, const PIString & v) {s << *(PIDeque<PIChar>*)&v; return s;}

//! \relatesalso PIString \relatesalso PIByteArray \brief Input operator from PIByteArray
inline PIByteArray & operator >>(PIByteArray & s, PIString & v) {s >> *(PIDeque<PIChar>*)&v; return s;}


//! \relatesalso PIString \brief Return concatenated string
inline PIString operator +(const PIString & str, const PIString & f) {PIString s(str); s += f; return s;}

//inline PIString operator +(const PIString & f, const char c) {PIString s(f); s.push_back(c); return s;}

//! \relatesalso PIString \brief Return concatenated string
inline PIString operator +(const PIString & f, const char * str) {PIString s(f); s += str; return s;}

//! \relatesalso PIString \brief Return concatenated string
inline PIString operator +(const PIString & f, const std::string & str) {PIString s(f); s += str; return s;}

//inline PIString operator +(const char c, const PIString & f) {return PIString(c) + f;}

//! \relatesalso PIString \brief Return concatenated string
inline PIString operator +(const char * str, const PIString & f) {return PIString(str) + f;}

//! \relatesalso PIString \brief Return concatenated string
inline PIString operator +(const std::string & str, const PIString & f) {return PIString(str) + f;}

inline char chrUpr(char c);
inline char chrLwr(char c);


/*!\brief Strings array class
 * \details This class is based on \a PIDeque<PIString> and
 * expand it functionality. */
class PIP_EXPORT PIStringList: public PIDeque<PIString>
{
public:
	
	//! Contructs empty strings list
	PIStringList() {;}
	
	//! Contructs strings list with one string "str"
	PIStringList(const PIString & str) {push_back(str);}
	
	//! Contructs empty strings list with strings "s0" and "s1"
	PIStringList(const PIString & s0, const PIString & s1) {push_back(s0); push_back(s1);}
	
	//! Contructs empty strings list with strings "s0", "s1" and "s2"
	PIStringList(const PIString & s0, const PIString & s1, const PIString & s2) {push_back(s0); push_back(s1); push_back(s2);}
	
	//! Contructs empty strings list with strings "s0", "s1", "s2" and "s3"
	PIStringList(const PIString & s0, const PIString & s1, const PIString & s2, const PIString & s3) {push_back(s0); push_back(s1); push_back(s2); push_back(s3);}
	
	PIStringList(const PIStringList & o): PIDeque<PIString>() {resize(o.size()); for (uint i = 0; i < size(); ++i) (*this)[i] = o[i];}
	PIStringList(const PIVector<PIString> & o): PIDeque<PIString>() {resize(o.size()); for (uint i = 0; i < size(); ++i) (*this)[i] = o[i];}
	PIStringList(const PIDeque<PIString> & o): PIDeque<PIString>() {resize(o.size()); for (uint i = 0; i < size(); ++i) (*this)[i] = o[i];}

	
	//! \brief Join all strings in one with delimiter "delim" and return it
	//! \details Example: \snippet pistring.cpp PIStringList::join
	PIString join(const PIString & delim) const {PIString s; for (uint i = 0; i < size(); ++i) {s += at(i); if (i < size() - 1) s += delim;} return s;}
	
	//! \brief Remove all strings equal "value" and return this
	//! \details Example: \snippet pistring.cpp PIStringList::removeStrings
	PIStringList & removeStrings(const PIString & value) {for (uint i = 0; i < size(); ++i) {if (at(i) == value) {remove(i); --i;}} return *this;}
	
	PIStringList & remove(uint num) {PIDeque<PIString>::remove(num); return *this;}
	PIStringList & remove(uint num, uint count) {PIDeque<PIString>::remove(num, count); return *this;}
	
	//! \brief Remove duplicated strings and return this
	//! \details Example: \snippet pistring.cpp PIStringList::removeDuplicates
	PIStringList & removeDuplicates();
	
	//! \brief Trim all strings
	//! \details Example: \snippet pistring.cpp PIStringList::trim
	PIStringList & trim() {for (uint i = 0; i < size(); ++i) at(i).trim(); return *this;}
	
	//! Return sum of lengths of all strings
	uint contentSize() {uint s = 0; for (uint i = 0; i < size(); ++i) s += at(i).size(); return s;}

	//! Compare operator
	bool operator ==(const PIStringList & o) const {if (size() != o.size()) return false; for (size_t i = 0; i < size(); ++i) if (o[i] != (*this)[i]) return false; return true;}

	//! Compare operator
	bool operator !=(const PIStringList & o) const {return !(o == (*this));}

	PIStringList & operator =(const PIStringList & o) {clear(); for (uint i = 0; i < o.size(); ++i) *this << o[i]; return *this;}

	PIStringList & operator <<(const PIString & str) {push_back(str); return *this;}
	PIStringList & operator <<(const PIStringList & sl) {piForeachC (PIString & i, sl) push_back(i); return *this;}
	//inline PIStringList & operator <<(const char c) {push_back(PIString(c)); return *this;}
	PIStringList & operator <<(const char * str) {push_back(PIString(str)); return *this;}
	PIStringList & operator <<(const std::string & str) {push_back(str); return *this;}
	PIStringList & operator <<(const int & num) {push_back(PIString::fromNumber(num)); return *this;}
	PIStringList & operator <<(const short & num) {push_back(PIString::fromNumber(num)); return *this;}
	PIStringList & operator <<(const long & num) {push_back(PIString::fromNumber(num)); return *this;}
	PIStringList & operator <<(const float & num) {push_back(PIString::fromNumber(num)); return *this;}
	PIStringList & operator <<(const double & num) {push_back(PIString::fromNumber(num)); return *this;}

};


//! \relatesalso PIStringList \relatesalso PIByteArray \brief Output operator to PIByteArray
inline PIByteArray & operator <<(PIByteArray & s, const PIStringList & v) {s << int(v.size_s()); for (int i = 0; i < v.size_s(); ++i) s << v[i]; return s;}

//! \relatesalso PIStringList \relatesalso PIByteArray \brief Input operator from PIByteArray
inline PIByteArray & operator >>(PIByteArray & s, PIStringList & v) {int sz; s >> sz; v.resize(sz); for (int i = 0; i < sz; ++i) s >> v[i]; return s;}


//! \relatesalso PIStringList \brief Output operator to std::ostream (cout)
inline std::ostream & operator <<(std::ostream & s, const PIStringList & v) {s << "{"; for (uint i = 0; i < v.size(); ++i) {s << "\"" << v[i] << "\""; if (i < v.size() - 1) s << ", ";} s << "}"; return s;}

//! \relatesalso PIStringList \relatesalso PICout \brief Output operator to PICout
inline PICout operator <<(PICout s, const PIStringList & v) {s.space(); s.setControl(0, true); s << "{"; for (uint i = 0; i < v.size(); ++i) {s << "\"" << v[i] << "\""; if (i < v.size() - 1) s << ", ";} s << "}"; s.restoreControl(); return s;}

#endif // PISTRING_H
