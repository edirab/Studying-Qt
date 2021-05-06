/*! \file piinit.h
 * \brief Initialization
 * 
 * 
*/
/*
    PIP - Platform Independent Primitives
    Initialization
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

#ifndef PIINIT_H
#define PIINIT_H

#include "piincludes.h"


class PIFile;


class __PIInit_Initializer__ {
public:
	__PIInit_Initializer__();
	~__PIInit_Initializer__();
	static int count_;
	static PIInit * __instance__;
};


class PIInit {
	friend class PIFile;
public:
	PIInit();
	~PIInit();

	//! \brief Build options which PIP library was built
	enum BuildOption {
		ICU /*! Unicode support */ = 0x01,
		USB /*! USB support */ = 0x02,
		STL /*! STL containers implementation */ = 0x04,
		Crypt /*! Crypt support */ = 0x08,
		IntrospectionContainers /*! Containers introspection */ = 0x010,
		IntrospectionThreads /*! Threads introspection */ = 0x20
	};
	static PIInit * instance() {return __PIInit_Initializer__::__instance__;}
	static bool isBuildOptionEnabled(BuildOption o);
private:
	void setFileCharset(const char *charset);
	bool fileExists(const PIString & p);
#ifdef WINDOWS
	HMODULE ntlib;
	ULONG prev_res;
#endif
	char * file_charset;
};

static __PIInit_Initializer__ __piinit_initializer__;


#endif // PIINIT_H
