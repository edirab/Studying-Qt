/*! \file pisignals.h
 * \brief System signals
*/
/*
    PIP - Platform Independent Primitives
    Signals
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

#ifndef PISIGNALS_H
#define PISIGNALS_H

#include "picontainers.h"

class PIP_EXPORT PISignals
{
public:
	enum Signal {
		Interrupt /** Interrupt from keyboard */ = 0x01, // Term Interrupt from keyboard
		Illegal /** Illegal Instruction */ = 0x02, // Core Illegal Instruction
		Abort /** Abort signal */ = 0x04, // Core Abort signal from abort
		FPE /** Floating point exception */ = 0x08, // Core Floating point exception
		SegFault /** Invalid memory reference */ = 0x10, // Core Invalid memory reference
		Termination /** Termination signal */ = 0x20, // Term Termination signal
		Hangup        = 0x40, // Term Hangup detected on controlling terminal or death of controlling process
		Quit          = 0x80, // Core Quit from keyboard
		Kill          = 0x100, // Term Kill signal
		BrokenPipe    = 0x200, // Term Broken pipe: write to pipe with no readers
		Timer         = 0x400, // Term Timer signal from alarm
		UserDefined1  = 0x800, // Term User-defined signal 1
		UserDefined2  = 0x1000, // Term User-defined signal 2
		ChildStopped  = 0x2000, // Ign  Child stopped or terminated
		Continue      = 0x4000, // Cont Continue if stopped
		StopProcess   = 0x8000, // Stop Stop process
		StopTTY       = 0x10000, // Stop Stop typed at tty
		StopTTYInput  = 0x20000, // Stop tty input for background process
		StopTTYOutput = 0x40000, // Stop tty output for background process
		All           = 0xFFFFF
	};

	typedef void (*SignalEvent)(PISignals::Signal);
	// slot is any function format "void <func>(PISignals::Signal)"
	static void setSlot(SignalEvent slot) {ret_func = slot;}
	static void grabSignals(PIFlags<PISignals::Signal> signals_);
	static void raiseSignal(PISignals::Signal signal);

private:
	PISignals() {ret_func = 0;}
	~PISignals() {}

	static int signalCode(PISignals::Signal signal);
	static PISignals::Signal signalFromCode(int signal);
	static void signal_event(int signal);

	static SignalEvent ret_func;

};


#endif // PISIGNALS_H
