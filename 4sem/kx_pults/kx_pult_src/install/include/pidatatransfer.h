/*! \file pidatatransfer.h
 * \brief Class for send and receive PIByteArray via \a PIBaseTransfer
*/
/*
	PIP - Platform Independent Primitives
	Class for send and receive PIByteArray via PIBaseTransfer
	Copyright (C) 2016 Andrey Bychkov work.a.b@yandex.ru

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

#ifndef PIDATATRANSFER_H
#define PIDATATRANSFER_H

#include "pibasetransfer.h"

class PIDataTransfer: public PIBaseTransfer
{
	PIOBJECT_SUBCLASS(PIDataTransfer, PIBaseTransfer)
public:
	PIDataTransfer() {;}
	~PIDataTransfer() {;}
	
	bool send(const PIByteArray &ba);
	const PIByteArray & data() {return data_;}
	
private:
	virtual PIByteArray buildPacket(Part p);
	virtual void receivePart(PIBaseTransfer::Part fi, PIByteArray ba, PIByteArray pheader);
	PIByteArray data_;
	
};


#endif // PIDATATRANSFER_H
