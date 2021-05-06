/*! \file picontainers.h
 * \brief Queue container
 * 
 * This file declare PIQueue
*/
/*
    PIP - Platform Independent Primitives
    Queue container
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

#ifndef PIQUEUE_H
#define PIQUEUE_H

#include "pideque.h"

template<typename T>
class PIP_EXPORT PIQueue: public PIDeque<T> {
public:
	PIQueue() {;}
	PIDeque<T> & enqueue(const T & v) {PIDeque<T>::push_front(v); return *this;}
	T dequeue() {return PIDeque<T>::take_back();}
	T & head() {return PIDeque<T>::back();}
	const T & head() const {return PIDeque<T>::back();}
	PIVector<T> toVector() {PIVector<T> v(PIDeque<T>::size()); for (uint i = 0; i < PIDeque<T>::size(); ++i) v[i] = PIDeque<T>::at(i); return v;}
};

#endif // PIQUEUE_H
