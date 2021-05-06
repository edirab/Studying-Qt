/*
    PIP - Platform Independent Primitives
    Process resource monitor
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

#ifndef PISYSTEMMONITOR_H
#define PISYSTEMMONITOR_H

#include "pithread.h"
#include "piprocess.h"
#ifdef WINDOWS
#  include <psapi.h>
#endif

class PIP_EXPORT PISystemMonitor: public PIThread
{
	PIOBJECT_SUBCLASS(PISystemMonitor, PIThread)
public:
	PISystemMonitor();
	~PISystemMonitor();

	struct ProcessStats {
		ProcessStats();
		PIString exec_name;
		PIString state;
		int ID;
		int parent_ID;
		int group_ID;
		int session_ID;
		int priority;
		int threads;
		ulong physical_memsize;
		ulong resident_memsize;
		ulong share_memsize;
		ulong virtual_memsize;
		ulong data_memsize;
		PIString physical_memsize_readable;
		PIString resident_memsize_readable;
		PIString share_memsize_readable;
		PIString virtual_memsize_readable;
		PIString data_memsize_readable;
		float cpu_load_system;
		float cpu_load_user;
	};

	bool startOnProcess(int pID);
	bool startOnSelf() {return startOnProcess(PIProcess::currentPID());}
	
	const ProcessStats & statistic() const {return stat;}
	void setStatistic(const ProcessStats & s) {stat = s; makeStrings();}

private:
	void stop();
	void run();
	void makeStrings();

	PIFile file, filem;
	ProcessStats stat;
	int pID_, page_size, cpu_count, cycle;
#ifndef WINDOWS
	llong cpu_u_cur, cpu_u_prev, cpu_s_cur, cpu_s_prev;
#else
	HANDLE hProc;
	PROCESS_MEMORY_COUNTERS mem_cnt;
	PISystemTime tm_kernel, tm_user;
	PITimeMeasurer tm;
#endif

};

#endif // PISYSTEMMONITOR_H
