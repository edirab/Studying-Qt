/*
    PIP - Platform Independent Primitives
    Counter of some PIP types
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

#ifndef PIMONITOR_H
#define PIMONITOR_H

#include "pip_export.h"

#if defined(DOXYGEN) || defined(__GNUC__)
#  undef PIP_EXPORT
#  define PIP_EXPORT
#endif

class PIP_EXPORT PIMonitor
{
public:
	PIMonitor();

	int containers, strings, threads, timers, serials, ethernets, files, objects;
	
};

#endif // PIMONITOR_H
