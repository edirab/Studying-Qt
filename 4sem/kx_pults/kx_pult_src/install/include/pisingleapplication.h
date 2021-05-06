/*
    PIP - Platform Independent Primitives
	Single application
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

#ifndef PISINGLEAPPLICATION_H
#define PISINGLEAPPLICATION_H

#include "pithread.h"

class PISharedMemory;

class PIP_EXPORT PISingleApplication: public PIThread {
	PIOBJECT_SUBCLASS(PISingleApplication, PIThread)
public:
	PISingleApplication(const PIString & app_name = PIString());
	~PISingleApplication();

	bool isFirst() const;

	EVENT_HANDLER1(void, sendMessage, const PIByteArray &, m);
	EVENT1(messageReceived, const PIByteArray &, m)

private:
	void begin();
	void run();
	void waitFirst() const;

	PISharedMemory * shm;
	PITimeMeasurer ftm;
	PIByteArray readed;
	bool first, started;
	int sacnt;

};

#endif // PISINGLEAPPLICATION_H
