/*
    PIP - Platform Independent Primitives
    Dynamic library
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

#ifndef PILIBRARY_H
#define PILIBRARY_H

#include "pistring.h"

class PIP_EXPORT PILibrary {
public:
	PILibrary(const PIString & path_ = PIString());
	~PILibrary();
	
	bool load(const PIString & path_);
	bool load();
	void unload();
	void * resolve(const char * symbol);
	
	bool isLoaded() const;
	PIString lastError() const {return liberror;}
	
private:
	bool loadInternal();
	void getLastError();
	
	PRIVATE_DECLARATION
	PIString libpath, liberror;
	
};

#endif // PILIBRARY_H
