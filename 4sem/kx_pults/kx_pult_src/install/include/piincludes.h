/*
    PIP - Platform Independent Primitives
    Minimal PIP includes
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

#ifndef PIINCLUDES_H
#define PIINCLUDES_H

#include "pibase.h"
#include "piflags.h"

#include <iostream>
#include <stdlib.h>
#include <cassert>
#include <string.h>
#include <string>
#ifndef QNX
#  include <cstdio>
#  include <cstdlib>
#else
#  include <stdio.h>
#endif
/*#ifdef WINDOWS
#  include <io.h>
#  include <windows.h>
#  include <wincon.h>
#endif*/
#ifdef CC_GCC
#  include <unistd.h>
#endif
/*
#include <sys/types.h>
#include <cctype>
#include <ctime>
#include <csignal>
#include <cmath>
#include <typeinfo>
#include <algorithm>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <stack>
#include <set>
#include <map>
#ifdef WINDOWS
#  include <conio.h>
#  include <io.h>
#  include <windows.h>
#  include <wincon.h>
#else
#  include <fcntl.h>
#  include <sys/ioctl.h>
#  include <pthread.h>
#endif
#ifdef MAC_OS
#  include <mach/mach_traps.h>
#  include <mach/mach.h>
#  include <mach/clock.h>
#  include <crt_externs.h>
#endif*/

extern PIMonitor piMonitor;

#ifndef QNX
  using std::wstring;
#else
  typedef std::basic_string<wchar_t> wstring;
#endif

class PIObject;
class PIMutex;
class PIString;
class PIInit;
struct lconv;

extern lconv * currentLocale;

/*! \fn errorString()
 * \brief Return readable error description in format "code <number> - <description>" */
PIString errorString();

void errorClear();

/// Return readable version of PIP
PIString PIPVersion();

#endif // PIINCLUDES_H
