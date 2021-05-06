/*! \file pitimer.h
 * \brief Timer
*/
/*
    PIP - Platform Independent Primitives
    Timer
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

#ifndef PITIMER_H
#define PITIMER_H

#include "pithread.h"
#include "pitime.h"


typedef void (*TimerEvent)(void * , int );

class PITimer;

class _PITimerBase {
	friend class PITimer;
public:
	_PITimerBase();
	virtual ~_PITimerBase() {stop();}
	
	double interval() const {return interval_;}
	void setInterval(double i);
	
	// ! \brief Return \c true if thread is running
	bool isRunning() const {return running_;}
	
	bool isStopped() const {return !running_;}
	
	bool start() {return start(interval_);}
	bool start(double interval_ms);
	void startDeferred(double delay_ms) {startDeferred(interval_, delay_ms);}
	void startDeferred(double interval_ms, double delay_ms);
	void startDeferred(PIDateTime start_datetime) {startDeferred(interval_, start_datetime);}
	void startDeferred(double interval_ms, PIDateTime start_datetime);
	
	bool stop();
	
	typedef void(*TickFunc)(PITimer*);
	TickFunc tfunc;
	PITimer * parent;
	
protected:
	
	virtual bool startTimer(double interval_ms) = 0;
	virtual bool stopTimer() = 0;
	
	double interval_, deferred_delay;
	bool deferred_, deferred_mode; // mode: true - date, false - delay
	volatile bool running_;
	PIDateTime deferred_datetime;
};



class _PITimerImp_Thread: public _PITimerBase {
public:
	_PITimerImp_Thread();
	virtual ~_PITimerImp_Thread();
protected:
	void prepareStart(double interval_ms);
	bool threadFunc(); // returns true if repeat is needed
	int wait_dt, wait_dd, wait_tick;
private:
	virtual bool startTimer(double interval_ms);
	virtual bool stopTimer();
	static void threadFuncS(void * d) {((_PITimerImp_Thread*)d)->threadFunc();}
	void adjustTimes();
	
	PIThread thread_;
	PISystemTime st_time, st_inc, st_wait, st_odt;
};


#ifdef PIP_TIMER_RT
struct _PITimerImp_RT_Private_;
class _PITimerImp_RT: public _PITimerBase {
public:
	_PITimerImp_RT();
	virtual ~_PITimerImp_RT();
protected:
private:
	virtual bool startTimer(double interval_ms);
	virtual bool stopTimer();

	int ti;
	_PITimerImp_RT_Private_ * priv;
};
#endif


class _PITimerImp_Pool: public _PITimerImp_Thread {
public:
	_PITimerImp_Pool();
	virtual ~_PITimerImp_Pool() {stop();}
private:
	class Pool: public PIThread {
	public:
		Pool();
		~Pool();
		static Pool * instance();
		void add(_PITimerImp_Pool * t);
		void remove(_PITimerImp_Pool * t);
		void run();
		PIVector<_PITimerImp_Pool * > timers, to_remove;
	};
	virtual bool startTimer(double interval_ms);
	virtual bool stopTimer();
};




class PITimer: public PIObject {
	PIOBJECT_SUBCLASS(PITimer, PIObject)
public:
	
	//! \brief Constructs timer with PITimer::Thread implementation
	explicit PITimer();

	//! \brief Timer implementations
	enum TimerImplementation {
		Thread /*! Timer works in his own thread. Intervals are measured by the system time */ = 0x01,
		ThreadRT /*! Using POSIX timer with SIGEV_THREAD notification. \attention Doesn`t support on Windows and Mac OS! */ = 0x02,
		// SignalRT /*!  */ = 0x03,
		Pool /*! Using single TimerPool for all timers with this implementation. TimerPool works as Thread implementation and
			  * sequentially executes all timers. \attention Use this implementation with care! */ = 0x04
	};
	
	//! \brief Constructs timer with "ti" implementation
	explicit PITimer(TimerImplementation ti);
	
	//! \brief Constructs timer with "slot" slot, "data" data and "ti" implementation
	explicit PITimer(TimerEvent slot, void * data = 0, TimerImplementation ti = Thread);
	
	PITimer(const PITimer & other);
	
	virtual ~PITimer();
	
	
	//! \brief Returns timer implementation
	PITimer::TimerImplementation implementation() const {return imp_mode;}
	
	//! \brief Returns timer loop delay in milliseconds
	double interval() const {return imp->interval_;}
	
	//! \brief Set timer loop delay in milliseconds
	EVENT_HANDLER1(void, setInterval, double, ms) {setProperty("interval", ms); imp->setInterval(ms);}
	
	//! \brief Returns if timer is started
	bool isRunning() const {return imp->running_;}
	
	//! \brief Returns if timer is not started
	bool isStopped() const {return !imp->running_;}
	
	EVENT_HANDLER0(bool, start) {return imp->start();}
	EVENT_HANDLER1(bool, start, int, interval_ms_i) {setInterval(double(interval_ms_i)); return imp->start(double(interval_ms_i));}
	EVENT_HANDLER1(bool, start, double, interval_ms_d) {setInterval(interval_ms_d); return imp->start(interval_ms_d);}
	EVENT_HANDLER0(bool, restart) {imp->stop(); return imp->start();}
	EVENT_HANDLER1(void, startDeferred, double, delay_ms) {imp->startDeferred(delay_ms);}
	EVENT_HANDLER2(void, startDeferred, double, interval_ms, double, delay_ms) {imp->startDeferred(interval_ms, delay_ms);}
	EVENT_HANDLER1(void, startDeferred, PIDateTime, start_datetime) {startDeferred(imp->interval_, start_datetime);}
	EVENT_HANDLER2(void, startDeferred, double, interval_ms, PIDateTime, start_datetime) {imp->startDeferred(interval_ms, start_datetime);}
	
	EVENT_HANDLER0(bool, stop) {return imp->stop();}
	bool waitForFinish() {return waitForFinish(-1);}
	bool waitForFinish(int timeout_msecs);
	
	//! \brief Set custom data
	void setData(void * data_) {data_t = data_;}
	
	//! \brief Set timer tick function
	void setSlot(TimerEvent slot) {ret_func = slot;}
	
	//! \brief Returns common data passed to tick functions
	void * data() const {return data_t;}

	void needLockRun(bool need) {lockRun = need;}
	EVENT_HANDLER0(void, lock) {mutex_.lock();}
	EVENT_HANDLER0(void, unlock) {mutex_.unlock();}
	
	//! \brief Add frequency delimiter \b delim with optional delimiter slot \b slot.
	void addDelimiter(int delim, TimerEvent slot = 0) {delims << Delimiter(slot, delim);}
	
	//! \brief Remove all frequency delimiters \b delim.
	void removeDelimiter(int delim) {for (int i = 0; i < delims.size_s(); ++i) if (delims[i].delim == delim) {delims.remove(i); i--;}}
	
	//! \brief Remove all frequency delimiters with slot \b slot.
	void removeDelimiter(TimerEvent slot) {for (int i = 0; i < delims.size_s(); ++i) if (delims[i].slot == slot) {delims.remove(i); i--;}}
	
	//! \brief Remove all frequency delimiters \b delim with slot \b slot.
	void removeDelimiter(int delim, TimerEvent slot) {for (int i = 0; i < delims.size_s(); ++i) if (delims[i].slot == slot && delims[i].delim == delim) {delims.remove(i); i--;}}
	
	void setDelimiterValue(int delim, int value) {for (int i = 0; i < delims.size_s(); ++i) if (delims[i].delim == delim) delims[i].tick = value;}
	void setDelimiterValue(TimerEvent slot, int value) {for (int i = 0; i < delims.size_s(); ++i) if (delims[i].slot == slot) delims[i].tick = value;}
	void setDelimiterValue(int delim, TimerEvent slot, int value) {for (int i = 0; i < delims.size_s(); ++i) if (delims[i].slot == slot && delims[i].delim == delim) delims[i].tick = value;}
	int delimiterValue(int delim) {for (int i = 0; i < delims.size_s(); ++i) if (delims[i].delim == delim) return delims[i].tick; return -1;}
	int delimiterValue(int delim, TimerEvent slot) {for (int i = 0; i < delims.size_s(); ++i) if (delims[i].slot == slot && delims[i].delim == delim) return delims[i].tick; return -1;}
	EVENT_HANDLER0(void, clearDelimiters) {delims.clear();}
	
	EVENT2(tickEvent, void * , data_, int, delimiter)
	
//! \handlers
//! \{
	
	/** \fn bool start()
	 * \brief Start timer with \a interval() loop delay
	 * \details Start execution of timer functions with frequency = 1 / msecs Hz. */
	
	/** \fn bool start(int msecs)
	 * \brief Start timer with \b msecs loop delay
	 * \details Start execution of timer functions with frequency = 1 / msecs Hz. */
	
	/** \fn bool start(double msecs)
	 * \brief Start timer with \b msecs loop delay
	 * \details Start execution of timer functions with frequency = 1. / msecs Hz.
	 * Instead of \a start(int msecs) function this variant allow start timer
	 * with frequencies more than 1 kHz */
	
	//! \fn bool restart()
	//! \brief Stop and start timer with \a interval() loop delay
	
	//! \fn bool stop()
	//! \brief Stop timer

	/** \fn void deferredStart(double delay_msecs)
	 * \brief Start timer with \b interval() loop delay after \b delay_msecs delay.
	 * \details Timer wait \b delay_msecs milliseconds and then normally starts with
	 * \b interval() loop delay. */

	/** \fn void deferredStart(const PIDateTime & start_datetime)
	 * \brief Start timer with \b interval() loop delay after \b start_datetime date and time.
	 * \details Timer wait until \b start_datetime and then normally starts with
	 * \b interval() loop delay. */

	/** \fn void deferredStart(double interval_msecs, double delay_msecs)
	 * \brief Start timer with \b interval_msecs loop delay after \b delay_msecs delay.
	 * \details Timer wait \b delay_msecs milliseconds and then normally starts with
	 * \b interval_msecs loop delay. */

	/** \fn void deferredStart(double interval_msecs, const PIDateTime & start_datetime)
	 * \brief Start timer with \b interval_msecs loop delay after \b start_datetime date and time.
	 * \details Timer wait until \b start_datetime and then normally starts with
	 * \b interval_msecs loop delay. */
	
	//! \fn void clearDelimiters()
	//! \brief Remove all frequency delimiters
	
//! \}
//! \events
//! \{
	
	/** \fn void tickEvent(void * data, int delimiter)
	 * \brief Raise on timer tick
	 * \details \b Data can be set with function \a setData(void * data) or from constructor.
	 * \b Delimiter is frequency delimiter, 1 for main loop. */
	
	
//! \}
	
protected:
	struct Delimiter {
		Delimiter(TimerEvent slot_ = 0, int delim_ = 1) {slot = slot_; delim = delim_; tick = 0;}
		TimerEvent slot;
		int delim;
		int tick;
	};
	
	void initFirst();
	void init();
	void destroy();

	static void tickImpS(PITimer * t) {t->tickImp();}
	void tickImp();
	
	//! Virtual timer execution function, similar to "slot" or event \a void timeout(void * data, int delimiter).
	//! By default is empty.
	virtual void tick(void * data_, int delimiter) {}
	
	void * data_t;
	volatile bool lockRun;
	PIMutex mutex_;
	TimerEvent ret_func;
	TimerImplementation imp_mode;
	PIVector<Delimiter> delims;
	
	_PITimerBase * imp;
	
};


#endif // PITIMER_H
