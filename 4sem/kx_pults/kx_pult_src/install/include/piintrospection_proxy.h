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

#ifndef PIINTROSPECTION_PROXY_H
#define PIINTROSPECTION_PROXY_H

#include "pibase.h"

class PIString;

void __PIIntrospection__registerThread(int id, short prior, const PIString & name);
void __PIIntrospection__unregisterThread(int id);
void __PIIntrospection__containerNew();
void __PIIntrospection__containerDelete();
void __PIIntrospection__containerAlloc(ullong cnt);
void __PIIntrospection__containerFree(ullong cnt);
void __PIIntrospection__containerUsed(ullong cnt);
void __PIIntrospection__containerUnused(ullong cnt);
void __PIIntrospection__start();

#ifdef PIP_INTROSPECTION_CONTAINERS
#  define PIINTROSPECTION_CONTAINER_NEW() __PIIntrospection__containerNew();
#  define PIINTROSPECTION_CONTAINER_DELETE() __PIIntrospection__containerDelete();
#  define PIINTROSPECTION_CONTAINER_USED(cnt) __PIIntrospection__containerUsed(cnt);
#  define PIINTROSPECTION_CONTAINER_UNUSED(cnt) __PIIntrospection__containerUnused(cnt);
#  define PIINTROSPECTION_CONTAINER_ALLOC(cnt) __PIIntrospection__containerAlloc(cnt);
#  define PIINTROSPECTION_CONTAINER_FREE(cnt) __PIIntrospection__containerFree(cnt);
#else
#  define PIINTROSPECTION_CONTAINER_NEW()
#  define PIINTROSPECTION_CONTAINER_DELETE()
#  define PIINTROSPECTION_CONTAINER_USED(tcnt)
#  define PIINTROSPECTION_CONTAINER_UNUSED(cnt)
#  define PIINTROSPECTION_CONTAINER_ALLOC(cnt)
#  define PIINTROSPECTION_CONTAINER_FREE(cnt)
#endif

#ifdef PIP_INTROSPECTION_THREADS
#  define PIINTROSPECTION_REGISTER_THREAD(id, pr, name) __PIIntrospection__registerThread(id, pr, name);
#  define PIINTROSPECTION_UNREGISTER_THREAD(id) __PIIntrospection__unregisterThread(id);
#  define PIINTROSPECTION_START __PIIntrospection__start();
#else
#  define PIINTROSPECTION_REGISTER_THREAD(id, pr, name)
#  define PIINTROSPECTION_UNREGISTER_THREAD(id)
#  define PIINTROSPECTION_START
#endif

#if defined(PIP_INTROSPECTION_CONTAINERS) || defined(PIP_INTROSPECTION_THREADS)
#  define PIINTROSPECTION_START __PIIntrospection__start();
#else
#  define PIINTROSPECTION_START
#endif

#endif // PIINTROSPECTION_PROXY_H
