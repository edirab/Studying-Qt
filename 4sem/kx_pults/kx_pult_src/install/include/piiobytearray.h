/*! \file piiobytearray.h
 * \brief PIIODevice wrapper around PIByteArray
*/
/*
    PIP - Platform Independent Primitives
	PIIODevice wrapper around PIByteArray
    Copyright (C) 2016  Ivan Pelipenko peri4ko@yandex.ru, Andrey Bychkov work.a.b@yandex.ru

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

#ifndef PIIOBYTEARRAY_H
#define PIIOBYTEARRAY_H

#include "piiodevice.h"


class PIP_EXPORT PIIOByteArray: public PIIODevice
{
	PIIODEVICE(PIIOByteArray)
public:
	
	//! Contructs %PIIOByteArray with \"buffer\" content and \"mode\" open mode
	PIIOByteArray(PIByteArray * buffer = 0, PIIODevice::DeviceMode mode = PIIODevice::ReadWrite);
	
	//! Contructs %PIIOByteArray with \"buffer\" content only for read
	PIIOByteArray(const PIByteArray & buffer);
	
	~PIIOByteArray() {closeDevice();}
	
	//! Returns content
	PIByteArray * byteArray() const {return data_;}
	
	//! Clear content buffer
	void clear() {if (data_) data_->clear(); pos = 0;}
	
	//! Open \"buffer\" content with \"mode\" open mode
	bool open(PIByteArray * buffer, PIIODevice::DeviceMode mode = PIIODevice::ReadWrite);
	
	//! Open \"buffer\" content only for read
	bool open(const PIByteArray & buffer);
	
	//! Returns if position is at the end of content
	bool isEnd() const {if (!data_) return true; return pos >= data_->size_s();}
	
	
	//! Move read/write position to \"position\"
	void seek(llong position) {pos = position;}
	
	//! Move read/write position to the begin of the string
	void seekToBegin() {if (data_) pos = 0;}
	
	//! Move read/write position to the end of the string
	void seekToEnd() {if (data_) pos = data_->size_s();}
	
	
	//! Insert data \"ba\" into content at current position
	int writeByteArray(const PIByteArray & ba);
	
protected:
	bool openDevice();
	int readDevice(void * read_to, int size);
	int writeDevice(const void * data_, int size);

	ssize_t pos;
	PIByteArray * data_;

};

#endif // PIIOBYTEARRAY_H
