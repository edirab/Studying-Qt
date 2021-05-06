/*! \file pistack.h
 * \brief Stack container
 * 
 * This file declare PIStack
*/
/*
    PIP - Platform Independent Primitives
    Stack container
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

#ifndef PISTACK_H
#define PISTACK_H

#include "pivector.h"

template<typename T>
class PIP_EXPORT PIStack: public PIVector<T> {
public:
	PIStack() {;}
	PIVector<T> & push(const T & v) {PIVector<T>::push_back(v); return *this;}
	T pop() {return PIVector<T>::take_back();}
	T & top() {return PIVector<T>::back();}
	const T & top() const {return PIVector<T>::back();}
	PIVector<T> toVector() {PIVector<T> v(PIVector<T>::size()); for (uint i = 0; i < PIVector<T>::size(); ++i) v[i] = PIVector<T>::at(i); return v;}
};

#endif // PISTACK_H
