/*! \file pimutex.h
 * \brief Mutex
*/
/*
    PIP - Platform Independent Primitives
    Mutex
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

#ifndef PIMUTEX_H
#define PIMUTEX_H

#include "piinit.h"
#ifdef BLACKBERRY
#  include <pthread.h>
#endif

class PIP_EXPORT PIMutex
{
public:
	
	//! Constructs unlocked mutex
	explicit PIMutex();
	
	~PIMutex();


	PIMutex & operator =(const PIMutex & other);
	
	
	//! \brief Lock mutex
	//! \details If mutex is unlocked it set to locked state and returns immediate.
	//! If mutex is already locked function blocks until mutex will be unlocked
	void lock();
	
	//! \brief Unlock mutex
	//! \details In any case this function returns immediate
	void unlock();
	
	//! \brief Try to lock mutex
	//! \details If mutex is unlocked it set to locked state and returns "true" immediate.
	//! If mutex is already locked function returns immediate an returns "false"
	bool tryLock();
	
	//! Returns if mutex is locked
	bool isLocked() const;
	
private:
	void init();
	void destroy();

#ifdef WINDOWS
	void *
#else
	pthread_mutex_t
#endif
		mutex;
	bool inited_;
	volatile bool locked;
	
};


class PIP_EXPORT PIMutexLocker
{
public:
	PIMutexLocker(PIMutex * m, bool condition = true): mutex(m), cond(condition) {if (cond) mutex->lock();}
	PIMutexLocker(PIMutex & m, bool condition = true): mutex(&m), cond(condition) {if (cond) mutex->lock();}
	~PIMutexLocker() {if (cond) mutex->unlock();}
private:
	PIMutex * mutex;
	volatile bool cond;
};

#endif // PIMUTEX_H
