/*! \file pivariant.h
 * \brief Variant type
 *
 * This file declares PIVariant
*/
/*
	PIP - Platform Independent Primitives
	Variant type
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

#ifndef PIVARIANT_H
#define PIVARIANT_H

#include "pistring.h"
#include "pibitarray.h"
#include "pitime.h"
#include "pimathbase.h"

#ifndef QNX
#  define CUSTOM_PIVARIANT
#endif

#ifdef CUSTOM_PIVARIANT

template<typename T>
class __PIVariantFunctions__ {
public:
	static PIString typeNameHelper() {return PIStringAscii("");}

	static bool isSimpleHelper() {return false;}
	template<typename C> static PIByteArray castHelper(PIByteArray ba) {return PIByteArray();}
	template<typename C> static C castVariant(const T & v) {return C();}
};

struct __PIVariantInfo__ {
	__PIVariantInfo__() {
		simple = false;
	}
	typedef PIByteArray(*castHelperFunc)(PIByteArray);
	PIMap<PIString, castHelperFunc> cast;
	PIString typeName;
	bool simple;
};

class __PIVariantInfoStorage__ {
public:
	__PIVariantInfoStorage__() {if (!map) map = new PIMap<PIString, __PIVariantInfo__ * >();}
	static __PIVariantInfoStorage__ * get() {static __PIVariantInfoStorage__ * r = new __PIVariantInfoStorage__(); return r;}
	static PIMap<PIString, __PIVariantInfo__ * > * map;
};


#define REGISTER_VARIANT_H(classname) \
template<> inline PIString __PIVariantFunctions__< classname >::typeNameHelper() {static PIString tn = PIStringAscii(#classname); return tn;}

#define REGISTER_VARIANT_CPP(classname) \
template <typename T> \
class __##classname##_PIVariantInitializer__ { \
public: \
	__##classname##_PIVariantInitializer__(const PIString & name) { \
		if (__PIVariantInfoStorage__::get()->map->contains(name)) \
			return; \
		__PIVariantInfo__ * vi = new __PIVariantInfo__(); \
		vi->typeName = name; \
		(*(__PIVariantInfoStorage__::get()->map))[name] = vi; \
	} \
};

#define INIT_VARIANT(classname) \
	__##classname##_PIVariantInitializer__< classname > __##classname##_pivariant_initializer__(#classname);

#define REGISTER_VARIANT(classname) \
	REGISTER_VARIANT_H(classname) \
	REGISTER_VARIANT_CPP(classname) \
	static INIT_VARIANT(classname)


#define REGISTER_VARIANT_CAST_H(classname_from, classname_to) \
template<> template<> inline \
classname_to __PIVariantFunctions__<classname_from>::castVariant<classname_to>(const classname_from & v);

#define REGISTER_VARIANT_CAST_CPP(classname_from, classname_to) \
template<> template<> inline \
PIByteArray __PIVariantFunctions__<classname_from>::castHelper<classname_to>(PIByteArray v) { \
	classname_from f; v >> f; \
	classname_to t = __PIVariantFunctions__<classname_from>::castVariant<classname_to>(f); \
	PIByteArray ret; ret << t; \
	return ret;} \
template <typename T, typename C> \
class __##classname_from##_##classname_to##_PIVariantCastInitializer__ { \
public: \
	__##classname_from##_##classname_to##_PIVariantCastInitializer__(const PIString & name, const PIString & cname) { \
		__PIVariantInfo__ * vi(__PIVariantInfoStorage__::get()->map->value(name, 0)); \
		if (!vi) { \
			piCout << "Warning! Using REGISTER_VARIANT_CAST("#classname_from", "#classname_to") before REGISTER_VARIANT("#classname_from"), ignore."; \
			return; \
		} \
		vi->cast[cname] = __PIVariantFunctions__<classname_from>::castHelper<classname_to>; \
	} \
}; \
static __##classname_from##_##classname_to##_PIVariantCastInitializer__< classname_from, classname_to > __##classname_from##_##classname_to##_pivariant_cast_initializer__(#classname_from, #classname_to); \
template<> template<> \
classname_to __PIVariantFunctions__<classname_from>::castVariant<classname_to>(const classname_from & v)

#define REGISTER_VARIANT_CAST(classname_from, classname_to) \
	REGISTER_VARIANT_CAST_H(classname_from, classname_to) \
	REGISTER_VARIANT_CAST_CPP(classname_from, classname_to)


#define REGISTER_VARIANT_CAST_SIMPLE(classname_from, classname_to) REGISTER_VARIANT_CAST(classname_from, classname_to) {return classname_to(v);}
#define REGISTER_VARIANT_CAST_SIMPLE_H(classname_from, classname_to) REGISTER_VARIANT_CAST_H(classname_from, classname_to)
#define REGISTER_VARIANT_CAST_SIMPLE_CPP(classname_from, classname_to) REGISTER_VARIANT_CAST_CPP(classname_from, classname_to) {return classname_to(v);}

#else

#define REGISTER_VARIANT_H(classname)
#define REGISTER_VARIANT_CPP(classname)
#define INIT_VARIANT(classname)
#define REGISTER_VARIANT(classname)
#define REGISTER_VARIANT_CAST_H(classname_from, classname_to)
#define REGISTER_VARIANT_CAST_CPP(classname_from, classname_to)
#define REGISTER_VARIANT_CAST(classname_from, classname_to)
#define REGISTER_VARIANT_CAST_SIMPLE(classname_from, classname_to)
#define REGISTER_VARIANT_CAST_SIMPLE_H(classname_from, classname_to)
#define REGISTER_VARIANT_CAST_SIMPLE_CPP(classname_from, classname_to)

#endif

class PIP_EXPORT PIVariant {
	friend PICout operator <<(PICout s, const PIVariant & v);
	friend PIByteArray & operator <<(PIByteArray & s, const PIVariant & v);
	friend PIByteArray & operator >>(PIByteArray & s, PIVariant & v);
public:

	//! Type of %PIVariant content
	enum Type {
		pivInvalid /** Invalid type , default type of empty contructor */ = 0 ,
		pivBool /** bool */ ,
		pivChar /** char */ ,
		pivUChar /** uchar */ ,
		pivShort /** short */ ,
		pivUShort /** ushort */ ,
		pivInt /** int */ ,
		pivUInt /** uint */ ,
		pivLLong /** llong */ ,
		pivULLong /** ullong */ ,
		pivFloat /** float */ ,
		pivDouble /** double */ ,
		pivLDouble /** ldouble */ ,
		pivComplexd /** complexd */ ,
		pivComplexld /** complexld */ ,
		pivBitArray /** PIBitArray */ ,
		pivByteArray /** PIByteArray */ ,
		pivString /** PIString */ ,
		pivStringList /** PIStringList */ ,
		pivTime /** PITime */ ,
		pivDate /** PIDate */ ,
		pivDateTime /** PIDateTime */ ,
		pivSystemTime /** PISystemTime */ ,
		pivCustom /** Custom */ = 0xFF
	};

	//! Empty constructor, \a type() will be set to \a Invalid
	PIVariant();

	PIVariant(const PIVariant & v);

	//! Constructs variant from string
	PIVariant(const char * v) {initType(PIString(v));}

	//! Constructs variant from boolean
	PIVariant(const bool v) {initType(v);}

	//! Constructs variant from char
	PIVariant(const char v) {initType(v);}

	//! Constructs variant from integer
	PIVariant(const uchar v) {initType(v);}

	//! Constructs variant from integer
	PIVariant(const short v) {initType(v);}

	//! Constructs variant from integer
	PIVariant(const ushort v) {initType(v);}

	//! Constructs variant from integer
	PIVariant(const int & v) {initType(v);}

	//! Constructs variant from integer
	PIVariant(const uint & v) {initType(v);}

	//! Constructs variant from integer
	PIVariant(const llong & v) {initType(v);}

	//! Constructs variant from integer
	PIVariant(const ullong & v) {initType(v);}

	//! Constructs variant from float
	PIVariant(const float & v) {initType(v);}

	//! Constructs variant from double
	PIVariant(const double & v) {initType(v);}

	//! Constructs variant from long double
	PIVariant(const ldouble & v) {initType(v);}

	//! Constructs variant from complex
	PIVariant(const complexd & v) {initType(v);}

	//! Constructs variant from complex
	PIVariant(const complexld & v) {initType(v);}

	//! Constructs variant from bit array
	PIVariant(const PIBitArray & v) {initType(v);}

	//! Constructs variant from byte array
	PIVariant(const PIByteArray & v) {initType(v);}

	//! Constructs variant from string
	PIVariant(const PIString & v) {initType(v);}

	//! Constructs variant from strings list
	PIVariant(const PIStringList & v) {initType(v);}

	//! Constructs variant from time
	PIVariant(const PITime & v) {initType(v);}

	//! Constructs variant from date
	PIVariant(const PIDate & v) {initType(v);}

	//! Constructs variant from date and time
	PIVariant(const PIDateTime & v) {initType(v);}

	//! Constructs variant from system time
	PIVariant(const PISystemTime & v) {initType(v);}


	//! Set variant content and type to string
	void setValue(const char * v) {setValue(PIString(v));}

	//! Set variant content and type to boolean
	void setValue(const bool v) {initType(v);}

	//! Set variant content and type to char
	void setValue(const char v) {initType(v);}

	//! Set variant content and type to integer
	void setValue(const uchar v) {initType(v);}

	//! Set variant content and type to integer
	void setValue(const short v) {initType(v);}

	//! Set variant content and type to integer
	void setValue(const ushort v) {initType(v);}

	//! Set variant content and type to integer
	void setValue(const int & v) {initType(v);}

	//! Set variant content and type to integer
	void setValue(const uint & v) {initType(v);}

	//! Set variant content and type to integer
	void setValue(const llong & v) {initType(v);}

	//! Set variant content and type to integer
	void setValue(const ullong & v) {initType(v);}

	//! Set variant content and type to float
	void setValue(const float & v) {initType(v);}

	//! Set variant content and type to double
	void setValue(const double & v) {initType(v);}

	//! Set variant content and type to long double
	void setValue(const ldouble & v) {initType(v);}

	//! Set variant content and type to complex
	void setValue(const complexd & v) {initType(v);}

	//! Set variant content and type to complex
	void setValue(const complexld & v) {initType(v);}

	//! Set variant content and type to bit array
	void setValue(const PIBitArray & v) {initType(v);}

	//! Set variant content and type to byte array
	void setValue(const PIByteArray & v) {initType(v);}

	//! Set variant content and type to string
	void setValue(const PIString & v) {initType(v);}

	//! Set variant content and type to strings list
	void setValue(const PIStringList & v) {initType(v);}

	//! Set variant content and type to time
	void setValue(const PITime & v) {initType(v);}

	//! Set variant content and type to date
	void setValue(const PIDate & v) {initType(v);}

	//! Set variant content and type to date and time
	void setValue(const PIDateTime & v) {initType(v);}

	//! Set variant content and type to system time
	void setValue(const PISystemTime & v) {initType(v);}


	bool toBool() const;
	int toInt() const;
	llong toLLong() const;
	float toFloat() const;
	double toDouble() const;
	ldouble toLDouble() const;
	complexd toComplexd() const;
	complexld toComplexld() const;
	PITime toTime() const;
	PIDate toDate() const;
	PIDateTime toDateTime() const;
	PISystemTime toSystemTime() const;
	PIString toString() const;
	PIStringList toStringList() const;
	PIBitArray toBitArray() const;
	PIByteArray toByteArray() const;


	/** \brief Returns variant content as custom type
	* \details In case of known types this function equivalent \a to<Type> function. \n
	* Otherwise returns content as type T. */
	template<typename T>
	T value() const {return getAsValue<T>(*this);}
	/*
	operator bool() const {return toBool();}
	operator char() const {return toInt();}
	operator uchar() const {return toInt();}
	operator short() const {return toInt();}
	operator ushort() const {return toInt();}
	operator int() const {return toInt();}
	operator uint() const {return toInt();}
	operator long() const {return toInt();}
	operator ulong() const {return toInt();}
	operator llong() const {return toLLong();}
	operator ullong() const {return (ullong)toLLong();}
	operator float() const {return toFloat();}
	operator double() const {return toDouble();}
	operator ldouble() const {return toLDouble();}
	operator complexd() const {return toComplexd();}
	operator complexld() const {return toComplexld();}
	operator PITime() const {return toTime();}
	operator PIDate() const {return toDate();}
	operator PIDateTime() const {return toDateTime();}
	operator PIString() const {return toString();}
	operator PIStringList() const {return toStringList();}
	operator PIBitArray() const {return toBitArray();}
	operator PIByteArray() const {return toByteArray();}
	operator const char*() const {return toString().data();}
	operator void*() const {return (void*)(toLLong());}
	*/

	//! Assign operator
	PIVariant & operator =(const PIVariant & v);
	//! Assign operator
	PIVariant & operator =(const char * v) {setValue(PIString(v)); return *this;}
	//! Assign operator
	PIVariant & operator =(const bool v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const char v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const uchar v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const short v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const ushort v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const int & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const uint & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const llong & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const ullong & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const float & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const double & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const ldouble & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const complexd & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const complexld & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const PIBitArray & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const PIByteArray & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const PIString & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const PIStringList & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const PITime & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const PIDate & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const PIDateTime & v) {setValue(v); return *this;}
	//! Assign operator
	PIVariant & operator =(const PISystemTime & v) {setValue(v); return *this;}


	//! Compare operator
	bool operator ==(const PIVariant & v) const;
	//! Compare operator
	bool operator !=(const PIVariant & v) const {return !(*this == v);}


	//! Returns type of variant content
	PIVariant::Type type() const {return _type;}

	//! Returns type name of variant content
	PIString typeName() const;


	//! Returns \b true if type is not Invalid
	bool isValid() const {return _type != PIVariant::pivInvalid;}


	/** \brief Returns new variant from custom type
	* \details In case of known types this function equivalent \a PIVariant(T) constructors. \n
	* Otherwise returns variant with content \a v and type Custom. */
	template <typename T>
	static PIVariant fromValue(const T & v) {
		PIVariant ret;
		ret.initType<T>(v);
		return ret;
	}

	static PIVariant fromValue(const PIByteArray & c, const PIString & type) {
		PIVariant ret;
#ifdef CUSTOM_PIVARIANT
		ret._info = __PIVariantInfoStorage__::get()->map->value(type, 0);
		if (!ret._info) {
#endif
			PIVariant::Type t = typeFromName(type);
			if (t == pivInvalid) {
				piCout << "Can`t initialize PIVariant from unregistered type \"" << type << "\"!";
				return ret;
			}
			ret._type = t;
#ifdef CUSTOM_PIVARIANT
		} else
			ret._type = pivCustom;
#endif
		ret._content = c;
		return ret;
	}


	//! Returns type from name
	static PIVariant::Type typeFromName(const PIString & tname);

	//! Returns type name
	static PIString typeName(PIVariant::Type type);

private:
	void destroy() {_content.clear();}
	template <typename T> inline static Type getType() {return pivCustom;}
	template <typename T> inline void initType(const T & v) {
		_content.clear();
		_content << v;
		_type = getType<T>();
#ifdef CUSTOM_PIVARIANT
		if (_type == pivCustom) {
			_info = __PIVariantInfoStorage__::get()->map->value(__PIVariantFunctions__<T>::typeNameHelper(), 0);
			if (!_info)
				piCout << "Can`t initialize PIVariant from unregistered type!";
		} else
			_info = 0;
#endif
	}
	template<typename T> inline static T getAsValue(const PIVariant & v) {
#ifdef CUSTOM_PIVARIANT
		if (v._content.isEmpty() || !v._info) return T();
		PIString cn = __PIVariantFunctions__<T>::typeNameHelper();
		//piCout << "gav" << cn;
		PIByteArray ba;
		if (cn == v._info->typeName) {
			ba = v._content;
		} else {
			__PIVariantInfo__::castHelperFunc cf = v._info->cast.value(cn);
			//piCout << "gav cast" << cf;
			if (!cf) return T();
			ba = cf(v._content);
		}
		T ret; ba >> ret;
		return ret;
#else
		return T();
#endif
	}

	PIByteArray _content;
	PIVariant::Type _type;
#ifdef CUSTOM_PIVARIANT
	__PIVariantInfo__ * _info;
#endif

};

template<> inline bool PIVariant::value() const {return toBool();}
template<> inline char PIVariant::value() const {return (char)toInt();}
template<> inline uchar PIVariant::value() const {return (uchar)toInt();}
template<> inline short PIVariant::value() const {return (short)toInt();}
template<> inline ushort PIVariant::value() const {return (ushort)toInt();}
template<> inline int PIVariant::value() const {return toInt();}
template<> inline uint PIVariant::value() const {return (uint)toInt();}
template<> inline llong PIVariant::value() const {return toLLong();}
template<> inline ullong PIVariant::value() const {return (ullong)toLLong();}
template<> inline float PIVariant::value() const {return toFloat();}
template<> inline double PIVariant::value() const {return toDouble();}
template<> inline ldouble PIVariant::value() const {return toLDouble();}
template<> inline complexd PIVariant::value() const {return toComplexd();}
template<> inline complexld PIVariant::value() const {return toComplexld();}
template<> inline void* PIVariant::value() const {return (void*)toLLong();}
template<> inline const char* PIVariant::value() const {return toString().data();}
template<> inline PITime PIVariant::value() const {return toTime();}
template<> inline PIDate PIVariant::value() const {return toDate();}
template<> inline PIDateTime PIVariant::value() const {return toDateTime();}
template<> inline PIString PIVariant::value() const {return toString();}
template<> inline PIStringList PIVariant::value() const {return toStringList();}
template<> inline PIBitArray PIVariant::value() const {return toBitArray();}
template<> inline PIByteArray PIVariant::value() const {return toByteArray();}

//template<> inline PIVariant PIVariant::fromValue(const char * v) {return PIVariant(PIString(v));}
template<> inline PIVariant PIVariant::fromValue(const bool & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const char & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const uchar & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const short & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const ushort & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const int & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const uint & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const llong & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const ullong & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const float & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const double & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const ldouble & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const complexd & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const complexld & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const PIBitArray & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const PIByteArray & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const PIString & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const PIStringList & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const PITime & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const PIDate & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const PIDateTime & v) {return PIVariant(v);}
template<> inline PIVariant PIVariant::fromValue(const PISystemTime & v) {return PIVariant(v);}

template<> inline PIVariant::Type PIVariant::getType<bool>() {return PIVariant::pivBool;}
template<> inline PIVariant::Type PIVariant::getType<char>() {return PIVariant::pivChar;}
template<> inline PIVariant::Type PIVariant::getType<uchar>() {return PIVariant::pivUChar;}
template<> inline PIVariant::Type PIVariant::getType<short>() {return PIVariant::pivShort;}
template<> inline PIVariant::Type PIVariant::getType<ushort>() {return PIVariant::pivUShort;}
template<> inline PIVariant::Type PIVariant::getType<int>() {return PIVariant::pivInt;}
template<> inline PIVariant::Type PIVariant::getType<uint>() {return PIVariant::pivUInt;}
template<> inline PIVariant::Type PIVariant::getType<llong>() {return PIVariant::pivLLong;}
template<> inline PIVariant::Type PIVariant::getType<ullong>() {return PIVariant::pivULLong;}
template<> inline PIVariant::Type PIVariant::getType<float>() {return PIVariant::pivFloat;}
template<> inline PIVariant::Type PIVariant::getType<double>() {return PIVariant::pivDouble;}
template<> inline PIVariant::Type PIVariant::getType<ldouble>() {return PIVariant::pivLDouble;}
template<> inline PIVariant::Type PIVariant::getType<complexd>() {return PIVariant::pivComplexd;}
template<> inline PIVariant::Type PIVariant::getType<complexld>() {return PIVariant::pivComplexld;}
template<> inline PIVariant::Type PIVariant::getType<PIBitArray>() {return PIVariant::pivBitArray;}
template<> inline PIVariant::Type PIVariant::getType<PIByteArray>() {return PIVariant::pivByteArray;}
template<> inline PIVariant::Type PIVariant::getType<PIString>() {return PIVariant::pivString;}
template<> inline PIVariant::Type PIVariant::getType<PIStringList>() {return PIVariant::pivStringList;}
template<> inline PIVariant::Type PIVariant::getType<PITime>() {return PIVariant::pivTime;}
template<> inline PIVariant::Type PIVariant::getType<PIDate>() {return PIVariant::pivDate;}
template<> inline PIVariant::Type PIVariant::getType<PIDateTime>() {return PIVariant::pivDateTime;}
template<> inline PIVariant::Type PIVariant::getType<PISystemTime>() {return PIVariant::pivSystemTime;}

REGISTER_VARIANT(bool)
REGISTER_VARIANT(char)
REGISTER_VARIANT(uchar)
REGISTER_VARIANT(short)
REGISTER_VARIANT(ushort)
REGISTER_VARIANT(int)
REGISTER_VARIANT(uint)
REGISTER_VARIANT(llong)
REGISTER_VARIANT(ullong)
REGISTER_VARIANT(float)
REGISTER_VARIANT(double)
REGISTER_VARIANT(ldouble)
REGISTER_VARIANT(complexd)
REGISTER_VARIANT(complexld)
REGISTER_VARIANT(PIBitArray)
REGISTER_VARIANT(PIByteArray)
REGISTER_VARIANT(PIString)
REGISTER_VARIANT(PIStringList)
REGISTER_VARIANT(PITime)
REGISTER_VARIANT(PIDate)
REGISTER_VARIANT(PIDateTime)
REGISTER_VARIANT(PISystemTime)

inline PIByteArray & operator <<(PIByteArray & s, const PIVariant & v) {
	s << v._content << int(v._type);
	if (v._type == PIVariant::pivCustom) {
#ifdef CUSTOM_PIVARIANT
		if (v._info) {
			s << v._info->typeName;
		} else {
			s << PIStringAscii("");
		}
#else
		s << PIStringAscii("");
#endif
	}
	return s;
}
inline PIByteArray & operator >>(PIByteArray & s, PIVariant & v) {
	int t(0);
	s >> v._content >> t;
	v._type = (PIVariant::Type)t;
	if (v._type == PIVariant::pivCustom) {
		PIString tn;
		s >> tn;
#ifdef CUSTOM_PIVARIANT
		PIByteArray vc = v._content;
		v = PIVariant::fromValue(vc, tn);
#endif
	}
	return s;
}

inline PICout operator <<(PICout s, const PIVariant & v) {
	s.space(); s.setControl(0, true);
	s << "PIVariant(" << v.typeName() << ", " << v.toString() << ")";
	s.restoreControl(); return s;
}


#endif // PIVARIANT_H
