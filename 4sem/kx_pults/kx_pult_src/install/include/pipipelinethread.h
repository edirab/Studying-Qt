/*! \file pipipelinethread.h
 * \brief Class for create multihread pipeline
*/
/*
	PIP - Platform Independent Primitives
	Class for create multihread pipeline
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

#ifndef PIPIPELINETHREAD_H
#define PIPIPELINETHREAD_H

#include "pithread.h"

template <typename Tin, typename Tout>
class PIPipelineThread : public PIThread
{
	PIOBJECT_SUBCLASS(PIPipelineThread, PIThread)
public:
	PIPipelineThread() {
		cnt = 0;
		max_size = 0;
		wait_next_pipe = false;
		next_overload = false;
	}
	~PIPipelineThread() {
		stop();
		if (!waitForFinish(1000)) terminate();
	}
	template <typename T>
	void connectTo(PIPipelineThread<Tout, T> * next) {
		CONNECT2(void, Tout, bool *, this, calculated, next, enqueue)
	}
	EVENT2(calculated, const Tout &, v, bool *, overload)
	void enqueue(const Tin &v) {enqueue(v, 0);}
	EVENT_HANDLER2(void, enqueue, const Tin &, v, bool *, overload) {
		mutex.lock();
		//piCoutObj << "enque" << overload;//max_size << in.size();
		if (max_size == 0 || in.size() < max_size) {
			in.enqueue(v);
			if (overload) *overload = false;
		} else {
			if (overload) *overload = true;
		}
		mutex.unlock();
	}
	const ullong * counterPtr() const {return &cnt;}
	ullong counter() const {return cnt;}
	bool isEmpty() {
		bool ret;
		mutex.lock();
		ret = in.isEmpty();
		mutex.unlock();
		return ret;
	}
	int queSize() {
		int ret;
		mutex.lock();
		ret = in.size();
		mutex.unlock();
		return ret;
	}
	void clear() {
		mutex.lock();
		in.clear();
		next_overload = false;
		mutex.unlock();
	}
	void stopCalc() {
		if (isRunning()) {
			stop();
			if (!waitForFinish(100)) terminate();
			mutex.unlock();
			mutex_l.unlock();
		}
	}
	Tout getLast() {
		Tout ret;
		mutex_l.lock();
		ret = last;
		mutex_l.unlock();
		return ret;
	}

	uint maxQueSize() {
		uint ret;
		mutex.lock();
		ret = max_size;
		mutex.unlock();
		return ret;
	}

	void setMaxQueSize(uint count) {
		mutex.lock();
		max_size = count;
		if (max_size > 0 && in.size() > max_size) in.resize(max_size);
		mutex.unlock();
	}

	bool isWaitNextPipe() {return wait_next_pipe;}
	void setWaitNextPipe(bool wait) {wait_next_pipe = wait;}

protected:
	virtual Tout calc(Tin &v, bool &ok) = 0;

private:
	void begin() {cnt = 0;}
	void run() {
		//piCoutObj << "run ...";
		mutex.lock();
		if (in.isEmpty()) {
			mutex.unlock();
			piMSleep(10);
			//piCoutObj << "run in empty";
			return;
		}
		if (next_overload && wait_next_pipe) {
			mutex.unlock();
			//piCoutObj << "wait" << &next_overload;
			calculated(last, &next_overload);
			piMSleep(10);
		} else {
			Tin t = in.dequeue();
			mutex.unlock();
			bool ok = true;
			Tout r = calc(t, ok);
			if (ok) {
				mutex_l.lock();
				last = r;
				mutex_l.unlock();
				cnt++;
//				next_overload = true;
				//piCoutObj << "calc ok" << &next_overload;
				calculated(r, &next_overload);
			}
		}
		//piCoutObj << "run ok";
	}
	PIMutex mutex;
	PIMutex mutex_l;
	bool wait_next_pipe;
	bool next_overload;
	uint max_size;
	ullong cnt;
	PIQueue<Tin> in;
	Tout last;
};

#endif // PIPIPELINETHREAD_H
