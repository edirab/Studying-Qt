/*
    PIP - Platform Independent Primitives
    System tests results (see system_test folder)
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

#ifndef PISYSTEMTESTS_H
#define PISYSTEMTESTS_H

#include "piconfig.h"

namespace PISystemTests {
	PIP_EXPORT extern long time_resolution_ns;
	PIP_EXPORT extern long time_elapsed_ns;
	PIP_EXPORT extern long usleep_offset_us;

	class PISystemTestReader {
	public:
		PISystemTestReader();
	};

	extern PISystemTestReader pisystestreader;
	
};

#endif // PISYSTEMTESTS_H
