/*
    PIP - Platform Independent Primitives
	Introspection module
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

#ifndef PIINTROSPECTION_H
#define PIINTROSPECTION_H

#include "pipeer.h"
#include "pimutex.h"


class PIP_EXPORT PIIntrospectionThreads
{
	friend class __PIIntrospectionThreads__;
public:
	PIIntrospectionThreads();

	struct ThreadInfo {
		ThreadInfo() {id = 0; priority = 0;}
		PIString name;
		int id;
		short priority;
	};

	void registerThread(int id, short prior, const PIString & name);
	void unregisterThread(int id);

	PIMap<int, ThreadInfo> threads;
	PIMutex mutex;
};


class PIP_EXPORT PIIntrospectionContainers
{
	friend class __PIIntrospectionContainers__;
public:
	PIIntrospectionContainers();

	void containerNew();
	void containerDelete();
	void containerAlloc(ullong cnt);
	void containerFree(ullong cnt);
	void containerUsed(ullong cnt);
	void containerUnused(ullong cnt);
	
	uint count;
	ullong bytes_allocated;
	ullong bytes_used;
	PIMutex mutex;
};


class PIP_EXPORT PIIntrospectionServer: public PIPeer
{
	PIOBJECT_SUBCLASS(PIIntrospectionServer, PIPeer)
	friend class __PIIntrospectionServer__;
	PIIntrospectionServer();
public:
	PIString appname;
private:
	EVENT_HANDLER(void, timerEvent);
	PIString genName();
	PITimer itimer;
};


class __PIIntrospectionContainers__ {
public:
	__PIIntrospectionContainers__() {if (!p) p = new PIIntrospectionContainers();}
	static PIIntrospectionContainers * get() {static __PIIntrospectionContainers__ * r = new __PIIntrospectionContainers__(); return r->p;}
	static PIIntrospectionContainers * p;
};

class __PIIntrospectionThreads__ {
public:
	__PIIntrospectionThreads__() {if (!p) p = new PIIntrospectionThreads();}
	static PIIntrospectionThreads * get() {static __PIIntrospectionThreads__ * r = new __PIIntrospectionThreads__(); return r->p;}
	static PIIntrospectionThreads * p;
};

class __PIIntrospectionServer__ {
public:
	__PIIntrospectionServer__() {if (!p) p = new PIIntrospectionServer();}
	~__PIIntrospectionServer__() {if (!p) return; delete p; p = 0;}
	static PIIntrospectionServer * get() {static __PIIntrospectionServer__ * r = new __PIIntrospectionServer__(); return r->p;}
	static PIIntrospectionServer * p;
};


inline PIByteArray & operator <<(PIByteArray & b, const PIIntrospectionContainers & v) {b << v.count << v.bytes_allocated << v.bytes_used; return b;}
inline PIByteArray & operator <<(PIByteArray & b, const PIIntrospectionThreads::ThreadInfo & v) {b << v.id << v.priority << v.name; return b;}

inline PIByteArray & operator >>(PIByteArray & b, PIIntrospectionContainers & v) {b >> v.count >> v.bytes_allocated >> v.bytes_used; return b;}
inline PIByteArray & operator >>(PIByteArray & b, PIIntrospectionThreads::ThreadInfo & v) {b >> v.id >> v.priority >> v.name; return b;}

#define PIINTROSPECTION_CONTAINERS __PIIntrospectionContainers__::get()
#define PIINTROSPECTION_THREADS __PIIntrospectionThreads__::get()
#define PIINTROSPECTION_SERVER __PIIntrospectionServer__::get()

#endif // PIINTROSPECTION_H
