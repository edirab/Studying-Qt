/*
    PIP - Platform Independent Primitives
    Text codings coder, based on "iconv"
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

#ifndef PICODEC_H
#define PICODEC_H

#include "piprocess.h"

class PIP_EXPORT PICodec: private PIProcess
{
	PIOBJECT(PICodec)
public:
	PICodec(): PIProcess() {setGrabOutput(true); tf = PIFile::openTemporary(PIIODevice::ReadWrite); tf.open();}
	PICodec(const PIString & from, const PIString & to): PIProcess() {setCodings(from, to); tf = PIFile::openTemporary(PIIODevice::ReadWrite);}
	~PICodec() {tf.remove();}

	void setFromCoding(const PIString & from) {c_from = from;}
	void setToCoding(const PIString & to) {c_to = to;}
	void setCodings(const PIString & from, const PIString & to) {c_from = from; c_to = to;}

	PIStringList availableCodecs();
	PIString encode(PIString & str) {return PIString(exec_iconv(c_from, c_to, str.toByteArray()));}
	PIString encode(const PIByteArray & str) {return PIString(exec_iconv(c_from, c_to, str));}
	PIString decode(PIString & str) {return PIString(exec_iconv(c_to, c_from, str.toByteArray()));}
	PIString decode(const PIByteArray & str) {return PIString(exec_iconv(c_to, c_from, str));}

private:
	PIByteArray exec_iconv(const PIString & from, const PIString & to, const PIByteArray & str);

	PIString c_from, c_to;
	PIFile tf;

};

#endif // PICODEC_H
