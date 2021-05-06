/*! \file piset.h
 * \brief Set container
 * 
 * This file declare PISet
*/
/*
    PIP - Platform Independent Primitives
    Set container
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

#ifndef PISET_H
#define PISET_H

#include "pimap.h"

/*! \brief Set of any type
 * \details This class used to store collection of unique elements
 * of any type. You can only add values to set with \a operator<< or
 * with function \a insert(). You can discover if value already in
 * set with \a operator[] or with function \a find(). These function
 * has logarithmic complexity.
 */
template <typename T>
class PIP_EXPORT PISet: public PIMap<T, uchar> {
	typedef PIMap<T, uchar> _CSet;
public:
	
	//! Contructs an empty set
	PISet() {}
	
	//! Contructs set with one element "value"
	PISet(const T & value) {_CSet::insert(value, 0);}
	
	//! Contructs set with elements "v0" and "v1"
	PISet(const T & v0, const T & v1) {_CSet::insert(v0, 0); _CSet::insert(v1, 0);}
	
	//! Contructs set with elements "v0", "v1" and "v2"
	PISet(const T & v0, const T & v1, const T & v2) {_CSet::insert(v0, 0); _CSet::insert(v1, 0); _CSet::insert(v2, 0);}
	
	//! Contructs set with elements "v0", "v1", "v2" and "v3"
	PISet(const T & v0, const T & v1, const T & v2, const T & v3) {_CSet::insert(v0, 0); _CSet::insert(v1, 0); _CSet::insert(v2, 0); _CSet::insert(v3, 0);}

	//! Contructs set from vector of elements
	PISet(const PIVector<T> & values) {
		if (values.isEmpty()) return;
		//_CSet::pim_content.resize(values.size_s());
		//_CSet::pim_index.resize(values.size_s());
		for (int i = 0; i < values.size_s(); ++i) {
			//_CSet::pim_index[i].index = i;
			//_CSet::pim_index[i].key = values[i];
			_CSet::insert(values[i], 0);
		}
		//_CSet::_sort();
	}

	typedef T key_type;
	
	PISet<T> & operator <<(const T & t) {_CSet::insert(t, 0); return *this;}
	PISet<T> & operator <<(const PISet<T> & other) {(*(_CSet*)this) << *((_CSet*)&other); return *this;}
	
	//! Returns if element "t" exists in this set
	bool operator [](const T & t) const {return _CSet::contains(t);}
	
	//! Returns if element "t" exists in this set
	PISet<T> & remove(const T & t) {_CSet::remove(t); return *this;}

	//! Unite set with "v"
	PISet<T> & unite(const PISet<T> & v) {
		for (typename PIMap<T, uchar>::const_iterator i = v.begin(); i != v.end(); ++i)
			_CSet::insert(i->first, 0);
		return *this;
	}

	//! Subtract set with "v"
	PISet<T> & subtract(const PISet<T> & v) {
		for (typename PIMap<T, uchar>::const_iterator i = v.begin(); i != v.end(); ++i)
			_CSet::remove(i->first);
		return *this;
	}

	//! Returns content of set as PIVector
	PIVector<T> toVector() const {PIVector<T> ret; for (typename _CSet::const_iterator i = _CSet::begin(); i != _CSet::end(); ++i) ret << (*i).first; return ret;}
};


template<typename Type>
inline PICout operator <<(PICout s, const PISet<Type> & v) {
	s.space();
	s.setControl(0, true);
	s << "{";
	bool first = true;
	for (typename PIMap<Type, uchar>::const_iterator i = v.begin(); i != v.end(); ++i) {
		if (!first)
			s << ", ";
		first = false;
		s << i->first;
	}
	s << "}";
	s.restoreControl();
	return s;
}

#endif // PISET_H
