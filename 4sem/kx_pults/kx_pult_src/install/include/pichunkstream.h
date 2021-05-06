/*! \file pichunkstream.h
 * \brief Binary serializator
*/
/*
    PIP - Platform Independent Primitives
    Binary serializator
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

#ifndef PICHUNKSTREAM_H
#define PICHUNKSTREAM_H

#include "pibytearray.h"


class PIChunkStream
{
public:
	
	//! Contructs stream for read from "data"
	PIChunkStream(const PIByteArray & data) {setSource(data);}
	
	//! Contructs stream for read or write to/from "data", or empty stream for write
	PIChunkStream(PIByteArray * data = 0) {setSource(data);}
	
	~PIChunkStream();
	
	template <typename T>
	struct Chunk {
		Chunk(int i, const T & d): id(i), data(d) {}
		int id;
		T data;
	};
	
	//! Returns chunk with ID "id" and value "data" for write to stream
	template <typename T> static Chunk<T> chunk(int id, const T & data) {return Chunk<T>(id, data);}
	
	void setSource(const PIByteArray & data);
	void setSource(PIByteArray * data);
	
	//! Returns internal buffer with written data
	PIByteArray data() const {return tmp_data;}
	
	//! Returns if there is end of stream
	bool atEnd() const {return data_->isEmpty();}
	
	
	//! Read one chunk from stream and returns its ID
	int read() {(*data_) >> last_id >> last_data; return last_id;}
	
	//! Returns last readed chunk ID
	int getID() {return last_id;}
	
	//! Returns value of last readed chunk
	template <typename T>
	T getData() const {T ret; PIByteArray s(last_data); s >> ret; return ret;}

	//! Place value of last readed chunk into "v"
	template <typename T>
	void get(T & v) const {v = getData<T>();}
	
private:
	void _init();
	
	int last_id;
	PIByteArray * data_, last_data, tmp_data;
	
	template <typename T> friend PIChunkStream & operator <<(PIChunkStream & s, const PIChunkStream::Chunk<T> & c);
};

template <typename T>
PIChunkStream & operator <<(PIChunkStream & s, const PIChunkStream::Chunk<T> & c) {
	PIByteArray ba;
	ba << c.data;
	(*(s.data_)) << c.id << ba;
	return s;
}

#endif // PICHUNKSTREAM_H
