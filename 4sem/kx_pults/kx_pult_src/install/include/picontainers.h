/*! \file picontainers.h
 * \brief Generic containers
 * 
 * This file declare all containers and useful macros
 * to use them
*/
/*
    PIP - Platform Independent Primitives
    Generic containers
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

#ifndef PICONTAINERS_H
#define PICONTAINERS_H

#include "picout.h"
#include <list>

template<typename Type0, typename Type1>
class PIP_EXPORT PIPair {
public:
	PIPair() {first = Type0(); second = Type1();}
	PIPair(const Type0 & value0, const Type1 & value1) {first = value0; second = value1;}
	Type0 first;
	Type1 second;
};
template<typename Type0, typename Type1>
inline bool operator <(const PIPair<Type0, Type1> & value0, const PIPair<Type0, Type1> & value1) {return value0.first < value1.first;}
template<typename Type0, typename Type1>
inline bool operator ==(const PIPair<Type0, Type1> & value0, const PIPair<Type0, Type1> & value1) {return (value0.first == value1.first) && (value0.second == value1.second);}
template<typename Type0, typename Type1>
inline bool operator !=(const PIPair<Type0, Type1> & value0, const PIPair<Type0, Type1> & value1) {return (value0.first != value1.first) || (value0.second != value1.second);}
template<typename Type0, typename Type1>
inline std::ostream & operator <<(std::ostream & s, const PIPair<Type0, Type1> & v) {s << "(" << v.first << ", " << v.second << ")"; return s;}
template<typename Type0, typename Type1>
inline PICout operator <<(PICout s, const PIPair<Type0, Type1> & v) {s.space(); s.setControl(0, true); s << "(" << v.first << ", " << v.second << ")"; s.restoreControl(); return s;}

#include "pivector.h"
#include "pistack.h"
#include "piqueue.h"
#include "pideque.h"
#include "pimap.h"
#include "piset.h"

#ifdef DOXYGEN

/*!\brief Macro for iterate any container
 * \details Use this macros instead of standard "for"
 * to get read/write access to each element of container.
 * Pass direction is direct \n
 * Example: \snippet picontainers.cpp foreach
 */
#  define piForeach(i,c)

/*!\brief Macro for iterate any container only for read
 * \details Use this macros instead of standard "for"
 * to get read access to each element of container.
 * Pass direction is direct \n
 * Example: \snippet picontainers.cpp foreachC
 */
#  define piForeachC(i,c)

/*!\brief Macro for iterate any container with reverse direction
 * \details Use this macros instead of standard "for"
 * to get read/write access to each element of container.
 * Pass direction is reverse \n
 * Example: \snippet picontainers.cpp foreachR
 */
#  define piForeachR(i,c)

/*!\brief Macro for iterate any container only for read with reverse direction
 * \details Use this macros instead of standard "for"
 * to get read access to each element of container.
 * Pass direction is reverse \n
 * Example: \snippet picontainers.cpp foreachCR
 */
#  define piForeachCR(i,c)

/*!\brief Macro for break from any piForeach* loop
 * \details \warning C++ ordinary "break" doesn`t work inside piForeach*
 * loops! Always use "piBreak" instead!
 */
#  define piBreak

#else

#  define piBreak {_for._end = true; break;}

#  define piForTimes(c) for(int _i##c = 0; _i##c < c; ++_i##c)

#ifdef CC_GCC

template<typename Type>
class _PIForeach {
public:
	_PIForeach(Type & t): _t(t), _break(false), _end(false) {_it = _t.begin();}
	typename Type::value_type _var;
	typename Type::iterator _it;
	Type & _t;
	bool _break, _end;
	inline bool isEnd() {return _it == _t.end();}
	inline void operator ++() {if (_end) _it = _t.end(); else _it++; _break = false;}
};

template<typename Type>
class _PIForeachR {
public:
	_PIForeachR(Type & t): _t(t), _break(false), _end(false) {_rit = _t.rbegin();}
	typename Type::value_type _var;
	typename Type::reverse_iterator _rit;
	Type & _t;
	bool _break, _end;
	inline bool isEnd() {return _rit == _t.rend();}
	inline void operator ++() {if (_end) _rit = _t.rend(); else _rit++; _break = false;}
};

template<typename Type>
class _PIForeachC {
public:
	_PIForeachC(const Type & t): _t(t), _break(false), _end(false) {_it = _t.begin();}
	typename Type::value_type _var;
	typename Type::const_iterator _it;
	const Type & _t;
	bool _break, _end;
	inline bool isEnd() {return _it == _t.end();}
	inline void operator ++() {if (_end) _it = _t.end(); else _it++; _break = false;}
};

template<typename Type>
class _PIForeachCR {
public:
	_PIForeachCR(const Type & t): _t(t), _break(false), _end(false) {_rit = _t.rbegin();}
	typename Type::value_type _var;
	typename Type::const_reverse_iterator _rit;
	const Type & _t;
	bool _break, _end;
	inline bool isEnd() {return _rit == _t.rend();}
	inline void operator ++() {if (_end) _rit = _t.rend(); else _rit++; _break = false;}
};

#define piForeach(i,c) for(_PIForeach<typeof(c)> _for(c); !_for.isEnd(); ++_for) \
							for(i(*_for._it); !_for._break; _for._break = true)
#define piForeachR(i,c) for(_PIForeachR<typeof(c)> _for(c); !_for.isEnd(); ++_for) \
							for(i(*_for._rit); !_for._break; _for._break = true)
#define piForeachA(i,c) for(_PIForeach<typeof(c)> _for(c); !_for.isEnd(); ++_for) \
							for(typeof(_for._var) & i(*_for._it); !_for._break; _for._break = true)
#define piForeachAR(i,c) for(_PIForeachR<typeof(c)> _for(c); !_for.isEnd(); ++_for) \
							for(typeof(_for._var) & i(*_for._rit); !_for._break; _for._break = true)
#define piForeachC(i,c) for(_PIForeachC<typeof(c)> _for(c); !_for.isEnd(); ++_for) \
							for(const i(*_for._it); !_for._break; _for._break = true)
#define piForeachCR(i,c) for(_PIForeachCR<typeof(c)> _for(c); !_for.isEnd(); ++_for) \
							for(const i(*_for._rit); !_for._break; _for._break = true)
#define piForeachCA(i,c) for(_PIForeachC<typeof(c)> _for(c); !_for.isEnd(); ++_for) \
							for(const typeof(_for._var) & i(*_for._it); !_for._break; _for._break = true)
#define piForeachCAR(i,c) for(_PIForeachCR<typeof(c)> _for(c); !_for.isEnd(); ++_for) \
							for(const typeof(_for._var) & i(*_for._rit); !_for._break; _for._break = true)

#define piForeachRA piForeachAR
#define piForeachAC piForeachCA
#define piForeachCRA piForeachCAR
#define piForeachARC piForeachCAR
#define piForeachACR piForeachCAR
#define piForeachRCA piForeachCAR
#define piForeachRAC piForeachCAR

#else

struct _PIForeachBase {mutable bool _break, _end;};

template<typename Type>
class _PIForeach: public _PIForeachBase {
public:
	_PIForeach(Type & t, bool i = false): _break(false), _end(false), _t(t), _inv(i) {if (_inv) _rit = _t.rbegin(); else _it = _t.begin();}
	mutable typename Type::value_type _var;
	mutable typename Type::iterator _it;
	mutable typename Type::reverse_iterator _rit;
	Type & _t;
	bool _inv;
	bool isEnd() {if (_inv) return _rit == _t.rend(); else return _it == _t.end();}
	void operator ++() {if (_inv) {if (_end) _rit = _t.rend(); else _rit++;} else {if (_end) _it = _t.end(); else _it++;} _break = false;}
};

template<typename Type>
class _PIForeachC: public _PIForeachBase {
public:
	_PIForeachC(const Type & t, bool i = false): _break(false), _end(false), _t(t), _inv(i) {if (_inv) _rit = _t.rbegin(); else _it = _t.begin();}
	mutable typename Type::value_type _var;
	mutable typename Type::const_iterator _it;
	mutable typename Type::const_reverse_iterator _rit;
	const Type & _t;
	bool _inv;
	bool isEnd() {if (_inv) return _rit == _t.rend(); else return _it == _t.end();}
	void operator ++() {if (_inv) {if (_end) _rit = _t.rend(); else _rit++;} else {if (_end) _it = _t.end(); else _it++;} _break = false;}
};

template <typename T> inline _PIForeach<T> _PIForeachNew(T & t, bool i = false) {return _PIForeach<T>(t, i);}
template <typename T> inline _PIForeach<T> * _PIForeachCast(_PIForeachBase & c, T & ) {return static_cast<_PIForeach<T> * >(&c);}

template <typename T> inline _PIForeachC<T> _PIForeachNewC(const T & t, bool i = false) {return _PIForeachC<T>(t, i);}
template <typename T> inline _PIForeachC<T> * _PIForeachCastC(_PIForeachBase & c, const T & ) {return static_cast<_PIForeachC<T> * >(&c);}

#define piForeach(i,c) for(_PIForeachBase & _for = _PIForeachNew(c); !_PIForeachCast(_for, c)->isEnd(); ++(*_PIForeachCast(_for, c))) \
							for(i = *(_PIForeachCast(_for, c)->_it); !_for._break; _for._break = true)
#define piForeachR(i,c) for(_PIForeachBase & _for = _PIForeachNew(c, true); !_PIForeachCast(_for, c)->isEnd(); ++(*_PIForeachCast(_for, c))) \
							for(i = *(_PIForeachCast(_for, c)->_rit); !_for._break; _for._break = true)
#define piForeachC(i,c) for(_PIForeachBase & _for = _PIForeachNewC(c); !_PIForeachCastC(_for, c)->isEnd(); ++(*_PIForeachCastC(_for, c))) \
							for(const i = *(_PIForeachCastC(_for, c)->_it); !_for._break; _for._break = true)
#define piForeachCR(i,c) for(_PIForeachBase & _for = _PIForeachNewC(c, false); !_PIForeachCastC(_for, c)->isEnd(); ++(*_PIForeachCastC(_for, c))) \
							for(const i = *(_PIForeachCastC(_for, c)->_rit); !_for._break; _for._break = true)

#endif

#define piForeachRC piForeachCR

#endif // DOXYGEN

template<typename Type, typename Allocator = std::allocator<Type> >
class PIP_EXPORT PIList: public std::list<Type, Allocator> {
	typedef PIList<Type, Allocator> _CList;
	typedef std::list<Type, Allocator> _stlc;
public:
	PIList() {piMonitor.containers++;}
	PIList(const Type & value) {piMonitor.containers++; _stlc::resize(1, value);}
	PIList(const Type & v0, const Type & v1) {piMonitor.containers++; _stlc::push_back(v0); _stlc::push_back(v1);}
	PIList(const Type & v0, const Type & v1, const Type & v2) {piMonitor.containers++; _stlc::push_back(v0); _stlc::push_back(v1); _stlc::push_back(v2);}
	PIList(const Type & v0, const Type & v1, const Type & v2, const Type & v3) {piMonitor.containers++; _stlc::push_back(v0); _stlc::push_back(v1); _stlc::push_back(v2); _stlc::push_back(v3);}
	PIList(uint size, const Type & value = Type()) {piMonitor.containers++; _stlc::resize(size, value);}
	~PIList() {piMonitor.containers--;}
	Type & operator [](uint index) {return (*this)[index];}
	Type & operator [](uint index) const {return (*this)[index];}
	const Type * data(uint index = 0) const {return &(*this)[index];}
	Type * data(uint index = 0) {return &(*this)[index];}
	int size_s() const {return static_cast<int>(_stlc::size());}
	bool isEmpty() const {return _stlc::empty();}
	bool has(const Type & t) const {for (typename _stlc::const_iterator i = _stlc::begin(); i != _stlc::end(); ++i) if (t == *i) return true; return false;}
	int etries(const Type & t) const {int ec = 0; for (typename _stlc::const_iterator i = _stlc::begin(); i != _stlc::end(); ++i) if (t == *i) ++ec; return ec;}
	_CList & fill(const Type & t) {_stlc::assign(_stlc::size(), t); return *this;}
	_CList & remove(uint index) {_stlc::erase(_stlc::begin() + index); return *this;}
	_CList & remove(uint index, uint count) {_stlc::erase(_stlc::begin() + index, _stlc::begin() + index + count); return *this;}
	_CList & insert(uint pos, const Type & t) {_stlc::insert(_stlc::begin() + pos, t); return *this;}
	_CList & operator <<(const Type & t) {_stlc::push_back(t); return *this;}
	PIVector<Type> toVector() const {PIVector<Type> v; for (typename _stlc::const_iterator i = _stlc::begin(); i != _stlc::end(); ++i) v << *i; return v;}
};


#ifndef PIP_CONTAINERS_STL

#  define __PICONTAINERS_SIMPLE_TYPE__(T) \
__PIDEQUE_SIMPLE_TYPE__(T)\
__PIVECTOR_SIMPLE_TYPE__(T)

__PICONTAINERS_SIMPLE_TYPE__(bool)
__PICONTAINERS_SIMPLE_TYPE__(char)
__PICONTAINERS_SIMPLE_TYPE__(uchar)
__PICONTAINERS_SIMPLE_TYPE__(short)
__PICONTAINERS_SIMPLE_TYPE__(ushort)
__PICONTAINERS_SIMPLE_TYPE__(int)
__PICONTAINERS_SIMPLE_TYPE__(uint)
__PICONTAINERS_SIMPLE_TYPE__(long)
__PICONTAINERS_SIMPLE_TYPE__(ulong)
__PICONTAINERS_SIMPLE_TYPE__(llong)
__PICONTAINERS_SIMPLE_TYPE__(ullong)
__PICONTAINERS_SIMPLE_TYPE__(float)
__PICONTAINERS_SIMPLE_TYPE__(double)
__PICONTAINERS_SIMPLE_TYPE__(ldouble)

#endif


#endif // PICONTAINERS_H
