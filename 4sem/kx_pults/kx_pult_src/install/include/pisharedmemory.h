/*! \file pisharedmemory.h
 * \brief Shared memory
*/
/*
    PIP - Platform Independent Primitives
	Shared Memory
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

#ifndef PISHAREDMEMORY_H
#define PISHAREDMEMORY_H

#include "piiodevice.h"


class PIP_EXPORT PISharedMemory: public PIIODevice
{
	PIIODEVICE(PISharedMemory)
public:
	
	explicit PISharedMemory();
		
	//! Constructs a shared memory object with name "shm_name", size "size" and open mode "mode"
	explicit PISharedMemory(const PIString & shm_name, int size, DeviceMode mode = ReadWrite);
	
	PISharedMemory(const PISharedMemory & other);
	
	virtual ~PISharedMemory() {close();}
	
	//! Read all shared memory object content to byte array and return it
	PIByteArray readAll();
	
	//! Returns shared memory object size
	llong size() const;
	
	//! Returns if shared memory object is empty
	bool isEmpty() const {return (size() <= 0);}
	
	//! Read from shared memory object to "read_to" no more than "max_size" and return readed bytes count
	int read(void * read_to, int max_size);

	//! Read from shared memory object to "read_to" no more than "max_size" and return readed bytes count
	int read(void * read_to, int max_size, int offset);
	
	//! Write to shared memory object "data" with size "max_size" and return written bytes count
	int write(const void * data, int max_size);

	//! Write to shared memory object "data" with size "max_size" and return written bytes count
	int write(const void * data, int max_size, int offset);

	//! Write "data" to shared memory object
	int write(const PIByteArray & data) {return write(data.data(), data.size_s());}

	//! Write "data" to shared memory object
	int write(const PIByteArray & data, int offset) {return write(data.data(), data.size_s(), offset);}
	

protected:
	bool openDevice();
	bool closeDevice();
	int readDevice(void * read_to, int max_size) {return read(read_to, max_size, 0);}
	int writeDevice(const void * data, int max_size) {return write(data, max_size, 0);}

private:
	void initPrivate();

	int dsize;
	PRIVATE_DECLARATION


};


#endif // PISHAREDMEMORY_H
