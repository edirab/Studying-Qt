/*! \file piiostring.h
 * \brief PIIODevice wrapper around PIString
*/
/*
    PIP - Platform Independent Primitives
    PIIODevice wrapper around PIString
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

#ifndef PIIOSTRING_H
#define PIIOSTRING_H

#include "piiodevice.h"


class PIP_EXPORT PIIOString: public PIIODevice
{
	PIIODEVICE(PIIOString)
public:
	
	//! Contructs %PIIOString with \"string\" content and \"mode\" open mode
	PIIOString(PIString * string = 0, PIIODevice::DeviceMode mode = PIIODevice::ReadWrite);
	
	//! Contructs %PIIOString with \"string\" content only for read
	PIIOString(const PIString & string);
	
	~PIIOString() {closeDevice();}
	
	//! Returns content
	PIString * string() const {return str;}
	
	//! Clear content string
	void clear() {if (str) str->clear(); pos = 0;}
	
	//! Open \"string\" content with \"mode\" open mode
	bool open(PIString * string, PIIODevice::DeviceMode mode = PIIODevice::ReadWrite);
	
	//! Open \"string\" content only for read
	bool open(const PIString & string);
	
	//! Returns if position is at the end of content
	bool isEnd() const {if (!str) return true; return pos >= str->size_s();}
	
	
	//! Move read/write position to \"position\"
	void seek(llong position) {pos = position;}
	
	//! Move read/write position to the begin of the string
	void seekToBegin() {if (str) pos = 0;}
	
	//! Move read/write position to the end of the string
	void seekToEnd() {if (str) pos = str->size_s();}
	
	
	//! Read one text line and return it
	PIString readLine();
	
	//! Insert string \"string\" into content at current position
	int writeString(const PIString & string);
	
protected:
	bool openDevice();
	int readDevice(void * read_to, int max_size);
	int writeDevice(const void * data, int max_size);

	ssize_t pos;
	PIString * str;

};

#endif // PIIOSTRING_H
