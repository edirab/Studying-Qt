/*! \file pithread.h
 * \brief Thread
 * 
 * This file declare thread class and some wait functions
*/
/*
    PIP - Platform Independent Primitives
    Thread
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

#ifndef PITHREAD_H
#define PITHREAD_H

#include "piinit.h"
#include "pimutex.h"
#include "piobject.h"
#ifdef BLACKBERRY
#  include <pthread.h>
#endif
#ifdef WINDOWS
#  define __THREAD_FUNC__ uint __stdcall
#else
#  define __THREAD_FUNC__ void*
#endif

typedef void (*ThreadFunc)(void * );

class PIP_EXPORT PIThread: public PIObject
{
	PIOBJECT_SUBCLASS(PIThread, PIObject)
public:
	
	//! Contructs thread with custom data "data", external function "func" and main loop delay "loop_delay".
	PIThread(void * data, ThreadFunc func, bool startNow = false, int loop_delay = -1);
	
	//! Contructs thread with main loop delay "loop_delay".
	PIThread(bool startNow = false, int loop_delay = -1);
	
	virtual ~PIThread();

	//! Priority of thread
	enum Priority {
		piLowerst /** Lowest */,
		piLow /** Low */,
		piNormal /** Normal, this is default priority of threads and timers */,
		piHigh /** High */,
		piHighest /** Highest */
	};

	EVENT_HANDLER0(bool, start) {return start(-1);}
	EVENT_HANDLER1(bool, start, int, timer_delay);
	EVENT_HANDLER1(bool, start, ThreadFunc, func) {ret_func = func; return start(-1);}
	EVENT_HANDLER2(bool, start, ThreadFunc, func, int, timer_delay) {ret_func = func; return start(timer_delay);}
	EVENT_HANDLER0(bool, startOnce);
	EVENT_HANDLER1(bool, startOnce, ThreadFunc, func) {ret_func = func; return startOnce();}
	EVENT_HANDLER0(void, stop) {stop(false);}
	EVENT_HANDLER1(void, stop, bool, wait) {terminating = true; if (wait) waitForFinish();}
	EVENT_HANDLER0(void, terminate);
	
	//! \brief Set common data passed to external function
	void setData(void * d) {data_ = d;}

	//! \brief Set external function that will be executed after every \a run()
	void setSlot(ThreadFunc func) {ret_func = func;}

	//! \brief Set priority of thread
	void setPriority(PIThread::Priority prior);
	
	//! \brief Returns common data passed to external function
	void * data() const {return data_;}

	//! \brief Return priority of thread
	PIThread::Priority priority() const {return priority_;}

	//! \brief Return \c true if thread is running
	bool isRunning() const {return running_;}
	
	bool isStopping() const {return running_ && terminating;}
	
	EVENT_HANDLER0(bool, waitForStart) {return waitForStart(-1);}
	EVENT_HANDLER1(bool, waitForStart, int, timeout_msecs);
	EVENT_HANDLER0(bool, waitForFinish) {return waitForFinish(-1);}
	EVENT_HANDLER1(bool, waitForFinish, int, timeout_msecs);

	//! \brief Set necessity of lock every \a run with internal mutex
	void needLockRun(bool need) {lockRun = need;}
	EVENT_HANDLER0(void, lock) {mutex_.lock();}
	EVENT_HANDLER0(void, unlock) {mutex_.unlock();}

	//! \brief Returns internal mutex
	PIMutex & mutex() {return mutex_;}

	//! \brief Returns thread ID
	llong tid() const {return tid_;}
	
	EVENT(started)
	EVENT(stopped)

//! \handlers
//! \{
	
	/** \fn bool start(int timer_delay = -1)
	 * \brief Start thread
	 * \details Start execution of \a run() in internal loop with
	 * "timer_delay" delay in milliseconds. If "timer_delay" <= 0
	 * there is no delay in loop. Thread also exec external function
	 * set by \a setSlot() if it`s not null
	 * 
	 * \return \c false if thread already started or can`t start thread */
	
	/** \fn bool start(ThreadFunc func, int timer_delay = -1)
	 * \brief Start thread
	 * \details Overloaded function. Set external function "func" before start
	 * 
	 * \return \c false if thread already started or can`t start thread */
	
	/** \fn bool startOnce()
	 * \brief Start thread without internal loop
	 * \details Start execution of \a run() once. Thread also exec
	 * external function set by \a setSlot() if it`s not null
	 * 
	 * \return \c false if thread already started or can`t start thread */
	
	/** \fn bool startOnce(ThreadFunc func)
	 * \brief Start thread without internal loop
	 * \details Overloaded function. Set external function "func" before start
	 * 
	 * \return \c false if thread already started or can`t start thread */
	
	/** \fn void stop(bool wait = false)
	 * \brief Stop thread
	 * \details Stop execution of thread and wait for it finish
	 * if "wait" is \c true. This function can block for infinite
	 * time if "wait" is \c true and any of thread function is
	 * busy forever */
	
	/** \fn void terminate()
	 * \brief Strongly stop thread
	 * \details Stop execution of thread immediately */
	
	/** \fn bool waitForStart(int timeout_msecs = -1)
	 * \brief Wait for thread start
	 * \details This function block until thread finish for "timeout_msecs"
	 * or forever if "timeout_msecs" < 0
	 * 
	 * \return \c false if timeout is exceeded */
	
	/** \fn bool waitForFinish(int timeout_msecs = -1)
	 * \brief Wait for thread finish
	 * \details This function block until thread start for "timeout_msecs"
	 * or forever if "timeout_msecs" < 0
	 * 
	 * \return \c false if timeout is exceeded */
	
	//! \fn void lock()
	//! \brief Lock internal mutex
	
	//! \fn void unlock()
	//! \brief Unlock internal mutex
	
//! \}
//! \events
//! \{
	
	//! \fn void started()
	//! \brief Raise on thread start
	
	//! \fn void stopped()
	//! \brief Raise on thread stop
	
//! \}

protected:
	static __THREAD_FUNC__ thread_function(void * t);
	static __THREAD_FUNC__ thread_function_once(void * t);
	static int priority2System(PIThread::Priority p);

	
	//! Function executed once at the start of thread.
	virtual void begin() {;}
	
	//! Function executed at every "timer_delay" msecs until thread was stopped.
	virtual void run() {;}
	
	//! Function executed once at the end of thread.
	virtual void end() {;}

	volatile bool terminating, running_, lockRun;
	int delay_, policy_;
	llong tid_;
	void * data_;
	PIMutex mutex_;
	PITimeMeasurer tmf_, tms_, tmr_;
	PIThread::Priority priority_;
	ThreadFunc ret_func;
#ifndef WINDOWS
	pthread_t thread;
	sched_param sparam;
#else
	void * thread;
#endif

};

#endif // PITHREAD_H
