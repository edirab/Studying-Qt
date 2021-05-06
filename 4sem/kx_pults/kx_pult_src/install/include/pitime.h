/*! \file pitime.h
 * \brief Time structs
*/
/*
    PIP - Platform Independent Primitives
    Time structs
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

#ifndef PITIME_H
#define PITIME_H

#include <ctime>
#include "pistring.h"
#ifdef QNX
#  include <time.h>
#endif

#ifdef DOXYGEN
//! \brief Sleep for "msecs" milliseconds
void msleep(int msecs);
#else
#  ifdef WINDOWS
inline void msleep(int msecs) {Sleep(msecs);}
#  else
inline void msleep(int msecs) {usleep(msecs * 1000);}
#  endif
#endif

/*! \brief Precise sleep for "usecs" microseconds
 * \details This function consider \c "usleep" offset
 * on QNX/Linux/Mac, which is calculated with
 * \a pip_sys_test program. If there is correct
 * offset value in system config, this function
 * wait \b exactly "usecs" microseconds. */
void piUSleep(int usecs); // on !Windows consider constant "usleep" offset

/*! \brief Precise sleep for "msecs" milliseconds
 * \details This function exec \a piUSleep (msecs * 1000). */
inline void piMSleep(double msecs) {piUSleep(int(msecs * 1000.));} // on !Windows consider constant "usleep" offset

/*! \brief Precise sleep for "secs" seconds
 * \details This function exec \a piUSleep (msecs * 1000000). */
inline void piSleep(double secs) {piUSleep(int(secs * 1000000.));} // on !Windows consider constant "usleep" offset

class PIP_EXPORT PISystemTime {
public:
	
	//! Contructs system time with s = ns = 0
	PISystemTime() {seconds = nanoseconds = 0;}
	
	//! Contructs system time with s = "s" and ns = "ns"
	PISystemTime(int s, int ns) {seconds = s; nanoseconds = ns; checkOverflows();}
	
	//! Contructs system time from another
	PISystemTime(const PISystemTime & t) {seconds = t.seconds; nanoseconds = t.nanoseconds;}
	
#ifdef WINDOWS
	PISystemTime(const FILETIME & t);
#endif
	
	//! Returns stored system time value in seconds
	double toSeconds() const {return double(seconds) + nanoseconds / 1.e+9;}
	
	//! Returns stored system time value in milliseconds
	double toMilliseconds() const {return seconds * 1.e+3 + nanoseconds / 1.e+6;}
	
	//! Returns stored system time value in microseconds
	double toMicroseconds() const {return seconds * 1.e+6 + nanoseconds / 1.e+3;}
	
	//! Returns stored system time value in nanoseconds
	double toNanoseconds() const {return seconds * 1.e+9 + double(nanoseconds);}
	
	
	//! Add to stored system time "v" seconds
	PISystemTime & addSeconds(double v) {*this += fromSeconds(v); return *this;}
	
	//! Add to stored system time "v" milliseconds
	PISystemTime & addMilliseconds(double v) {*this += fromMilliseconds(v); return *this;}
	
	//! Add to stored system time "v" microseconds
	PISystemTime & addMicroseconds(double v) {*this += fromMicroseconds(v); return *this;}
	
	//! Add to stored system time "v" nanoseconds
	PISystemTime & addNanoseconds(double v) {*this += fromNanoseconds(v); return *this;}
	
	
	//! Sleep for stored value. \warning Use this function to sleep for difference of system times or constructs system time.
	//! If you call this function on system time returned with \a PISystemTime::current() thread will be sleep almost forever.
	void sleep() {piUSleep(piFloord(toMicroseconds()));} // wait self value, useful to wait some dT = (t1 - t0)
	
	
	//! Returns copy of this system time with absolutely values of s and ns
	PISystemTime abs() const;
	
	//! Returns sum of this system time with "t"
	PISystemTime   operator +(const PISystemTime & t) const {PISystemTime tt(*this); tt.seconds += t.seconds; tt.nanoseconds += t.nanoseconds; tt.checkOverflows(); return tt;}
	
	//! Returns difference between this system time and "t"
	PISystemTime   operator -(const PISystemTime & t) const {PISystemTime tt(*this); tt.seconds -= t.seconds; tt.nanoseconds -= t.nanoseconds; tt.checkOverflows(); return tt;}
	
	//! Returns multiplication between this system time and "t"
	PISystemTime   operator *(const double & v) const {return fromMilliseconds(toMilliseconds() * v);}
	
	//! Returns division between this system time and "t"
	PISystemTime   operator /(const double & v) const {return fromMilliseconds(toMilliseconds() / v);}
	
	//! Add to stored value system time "t"
	PISystemTime & operator +=(const PISystemTime & t) {seconds += t.seconds; nanoseconds += t.nanoseconds; checkOverflows(); return *this;}
	
	//! Subtract from stored value system time "t"
	PISystemTime & operator -=(const PISystemTime & t) {seconds -= t.seconds; nanoseconds -= t.nanoseconds; checkOverflows(); return *this;}
	
	//! Multiply stored value system time by "v"
	PISystemTime & operator *=(const double & v) {*this = fromMilliseconds(toMilliseconds() * v); return *this;}
	
	//! Divide stored value system time by "v"
	PISystemTime & operator /=(const double & v) {*this = fromMilliseconds(toMilliseconds() / v); return *this;}
	
	
	//! Compare system times
	bool operator ==(const PISystemTime & t) const {return ((seconds == t.seconds) && (nanoseconds == t.nanoseconds));}
	
	//! Compare system times
	bool operator !=(const PISystemTime & t) const {return ((seconds != t.seconds) || (nanoseconds != t.nanoseconds));}
	
	//! Compare system times
	bool operator >(const PISystemTime & t) const {if (seconds == t.seconds) return nanoseconds > t.nanoseconds; return seconds > t.seconds;}
	
	//! Compare system times
	bool operator <(const PISystemTime & t) const {if (seconds == t.seconds) return nanoseconds < t.nanoseconds; return seconds < t.seconds;}
	
	//! Compare system times
	bool operator >=(const PISystemTime & t) const {if (seconds == t.seconds) return nanoseconds >= t.nanoseconds; return seconds >= t.seconds;}
	
	//! Compare system times
	bool operator <=(const PISystemTime & t) const {if (seconds == t.seconds) return nanoseconds <= t.nanoseconds; return seconds <= t.seconds;}
	
	
	//! Contructs system time from seconds "v"
	static PISystemTime fromSeconds(double v) {int s = piFloord(v); return PISystemTime(s, int((v - s) * 1000000000.));}
	
	//! Contructs system time from milliseconds "v"
	static PISystemTime fromMilliseconds(double v) {int s = piFloord(v / 1000.); return PISystemTime(s, int((v / 1000. - s) * 1000000000.));}
	
	//! Contructs system time from microseconds "v"
	static PISystemTime fromMicroseconds(double v) {int s = piFloord(v / 1000000.); return PISystemTime(s, int((v / 1000000. - s) * 1000000000.));}
	
	//! Contructs system time from nanoseconds "v"
	static PISystemTime fromNanoseconds(double v) {int s = piFloord(v / 1000000000.); return PISystemTime(s, int((v / 1000000000. - s) * 1000000000.));}
	
	//! Returns current system time
	static PISystemTime current(bool precise_but_not_system = false);
	
	//! Seconds of stored system time
	int seconds;
	
	//! Nanoseconds of stored system time
	int nanoseconds;
	
private:
	void checkOverflows() {while (nanoseconds >= 1000000000) {nanoseconds -= 1000000000; seconds++;} while (nanoseconds < 0) {nanoseconds += 1000000000; seconds--;}}
	
};

//! \relatesalso PICout \relatesalso PIByteArray \brief Output operator to PICout
inline PICout operator <<(PICout s, const PISystemTime & v) {s.space(); s.setControl(0, true); s << "(" << v.seconds << " s, " << v.nanoseconds << " ns)"; s.restoreControl(); return s;}

//! \relatesalso PISystemTime \relatesalso PIByteArray \brief Output operator to PIByteArray
inline PIByteArray & operator <<(PIByteArray & s, const PISystemTime & v) {s << v.seconds << v.nanoseconds; return s;}

//! \relatesalso PISystemTime \relatesalso PIByteArray \brief Input operator from PIByteArray
inline PIByteArray & operator >>(PIByteArray & s, PISystemTime & v) {s >> v.seconds >> v.nanoseconds; return s;}

struct PIP_EXPORT PITime {
	PITime(int hours_ = 0, int minutes_ = 0, int seconds_ = 0, int milliseconds_ = 0): hours(hours_), minutes(minutes_), seconds(seconds_), milliseconds(milliseconds_) {;}
	int hours;
	int minutes;
	int seconds;
	int milliseconds;
	PIString toString(const PIString & format = "h:mm:ss") const;
	PISystemTime toSystemTime() const;
	static PITime current();
	static PITime fromSystemTime(const PISystemTime & st);
};
PIP_EXPORT bool operator ==(const PITime & t0, const PITime & t1);
PIP_EXPORT bool operator <(const PITime & t0, const PITime & t1);
PIP_EXPORT bool operator >(const PITime & t0, const PITime & t1);
inline bool operator !=(const PITime & t0, const PITime & t1) {return !(t0 == t1);}
inline bool operator <=(const PITime & t0, const PITime & t1) {return !(t0 > t1);}
inline bool operator >=(const PITime & t0, const PITime & t1) {return !(t0 < t1);}
inline PIByteArray & operator <<(PIByteArray & s, const PITime & v) {s << v.hours << v.minutes << v.seconds << v.milliseconds; return s;}
inline PIByteArray & operator >>(PIByteArray & s, PITime & v) {s >> v.hours >> v.minutes >> v.seconds >> v.milliseconds; return s;}

struct PIP_EXPORT PIDate {
	PIDate(int year_ = 0, int month_ = 0, int day_ = 0): year(year_), month(month_), day(day_) {;}
	int year;
	int month;
	int day;
	PIString toString(const PIString & format = "d.MM.yyyy") const;
	static PIDate current();
};
PIP_EXPORT bool operator ==(const PIDate & t0, const PIDate & t1);
PIP_EXPORT bool operator <(const PIDate & t0, const PIDate & t1);
PIP_EXPORT bool operator >(const PIDate & t0, const PIDate & t1);
inline bool operator !=(const PIDate & t0, const PIDate & t1) {return !(t0 == t1);}
inline bool operator <=(const PIDate & t0, const PIDate & t1) {return !(t0 > t1);}
inline bool operator >=(const PIDate & t0, const PIDate & t1) {return !(t0 < t1);}
inline PIByteArray & operator <<(PIByteArray & s, const PIDate & v) {s << v.year << v.month << v.day; return s;}
inline PIByteArray & operator >>(PIByteArray & s, PIDate & v) {s >> v.year >> v.month >> v.day; return s;}

struct PIP_EXPORT PIDateTime {
	PIDateTime() {year = month = day = hours = minutes = seconds = milliseconds = 0;}
	PIDateTime(const PITime & time) {year = month = day = 0; hours = time.hours; minutes = time.minutes; seconds = time.seconds; milliseconds = time.milliseconds;}
	PIDateTime(const PIDate & date) {year = date.year; month = date.month; day = date.day; hours = minutes = seconds = milliseconds = 0;}
	PIDateTime(const PIDate & date, const PITime & time) {year = date.year; month = date.month; day = date.day; hours = time.hours; minutes = time.minutes; seconds = time.seconds; milliseconds = time.milliseconds;}
#ifdef WINDOWS
	PIDateTime(FILETIME t);
	FILETIME toFILETIME() const;
#endif
	int year;
	int month;
	int day;
	int hours;
	int minutes;
	int seconds;
	int milliseconds;
	PIDateTime normalized() const {return PIDateTime::fromSecondSinceEpoch(toSecondSinceEpoch());}
	void normalize() {*this = normalized();}
	PIString toString(const PIString & format = "h:mm:ss d.MM.yyyy") const;
	time_t toSecondSinceEpoch() const;
	PISystemTime toSystemTime() const {return PISystemTime(int(toSecondSinceEpoch()), milliseconds * 1000000);}
	PIDate date() const {return PIDate(year, month, day);}
	PITime time() const {return PITime(hours, minutes, seconds, milliseconds);}
	void setDate(const PIDate & d) {year = d.year; month = d.month; day = d.day;}
	void setTime(const PITime & t) {hours = t.hours; minutes = t.minutes; seconds = t.seconds; milliseconds = t.milliseconds;}
	void operator +=(const PIDateTime & d1) {year += d1.year; month += d1.month; day += d1.day; hours += d1.hours; minutes += d1.minutes; seconds += d1.seconds; normalize();}
	void operator -=(const PIDateTime & d1) {year -= d1.year; month -= d1.month; day -= d1.day; hours -= d1.hours; minutes -= d1.minutes; seconds -= d1.seconds; normalize();}
	static PIDateTime fromSecondSinceEpoch(const time_t sec);
	static PIDateTime fromSystemTime(const PISystemTime & st) {PIDateTime dt = fromSecondSinceEpoch(st.seconds); dt.milliseconds = piClampi(st.nanoseconds / 1000000, 0, 999); return dt;}
	static PIDateTime current();
};
inline PIDateTime operator +(const PIDateTime & d0, const PIDateTime & d1) {PIDateTime td = d0; td += d1; return td.normalized();}
inline PIDateTime operator -(const PIDateTime & d0, const PIDateTime & d1) {PIDateTime td = d0; td -= d1; return td.normalized();}
PIP_EXPORT bool operator ==(const PIDateTime & t0, const PIDateTime & t1);
PIP_EXPORT bool operator <(const PIDateTime & t0, const PIDateTime & t1);
PIP_EXPORT bool operator >(const PIDateTime & t0, const PIDateTime & t1);
inline bool operator !=(const PIDateTime & t0, const PIDateTime & t1) {return !(t0 == t1);}
inline bool operator <=(const PIDateTime & t0, const PIDateTime & t1) {return !(t0 > t1);}
inline bool operator >=(const PIDateTime & t0, const PIDateTime & t1) {return !(t0 < t1);}
inline PIByteArray & operator <<(PIByteArray & s, const PIDateTime & v) {s << v.year << v.month << v.day << v.hours << v.minutes << v.seconds << v.milliseconds; return s;}
inline PIByteArray & operator >>(PIByteArray & s, PIDateTime & v) {s >> v.year >> v.month >> v.day >> v.hours >> v.minutes >> v.seconds >> v.milliseconds; return s;}

DEPRECATED inline PITime currentTime() {return PITime::current();} // obsolete, use PITime::current() instead
DEPRECATED inline PIDate currentDate() {return PIDate::current();} // obsolete, use PIDate::current() instead
DEPRECATED inline PIDateTime currentDateTime() {return PIDateTime::current();} // obsolete, use PIDateTime::current() instead

//! \brief Returns current system time \deprecated Use \a PISystemTime::current() instead
DEPRECATED inline PISystemTime currentSystemTime() {return PISystemTime::current();} // obsolete, use PISystemTime::current() instead
DEPRECATED PIP_EXPORT PIString time2string(const PITime & time, const PIString & format = "h:mm:ss"); // obsolete, use PITime.toString() instead
DEPRECATED PIP_EXPORT PIString date2string(const PIDate & date, const PIString & format = "d.MM.yyyy"); // obsolete, use PITime.toString() instead
DEPRECATED PIP_EXPORT PIString datetime2string(const PIDateTime & datetime, const PIString & format = "h:mm:ss d.MM.yyyy"); // obsolete, use PIDateTime.toString() instead


class PITimeMeasurer {
public:
	PITimeMeasurer();
	
	/** \brief Set internal time mark to current system time
	 * \details This function used for set start time mark. Later
	 * you can find out elapsed time from this time mark to any
	 * moment of time with \a elapsed_s(), \a elapsed_m(),
	 * \a elapsed_u() or \a elapsed_n() functions.
	 * \sa \a elapsed_s(), \a elapsed_m(), \a elapsed_u(), \a elapsed_n() */
	void reset() {t_st = PISystemTime::current(true);}
	
	//! \brief Returns nanoseconds elapsed from last \a reset() execution or from timer measurer creation.
	double elapsed_n() const;

	//! \brief Returns microseconds elapsed from last \a reset() execution or from timer measurer creation.
	double elapsed_u() const;

	//! \brief Returns milliseconds elapsed from last \a reset() execution or from timer measurer creation.
	double elapsed_m() const;

	//! \brief Returns seconds elapsed from last \a reset() execution or from timer measurer creation.
	double elapsed_s() const;

	//! \brief Returns PISystemTime elapsed from last \a reset() execution or from timer measurer creation.
	PISystemTime elapsed() const;
	
	double reset_time_n() const {return t_st.toNanoseconds();}
	double reset_time_u() const {return t_st.toMicroseconds();}
	double reset_time_m() const {return t_st.toMilliseconds();}
	double reset_time_s() const {return t_st.toSeconds();}
	
	//! \brief Returns time mark of last \a reset() execution or timer measurer creation.
	PISystemTime reset_time() {return t_st;}
	
	//! \brief Returns nanoseconds representation of current system time.
	static double elapsed_system_n() {return PISystemTime::current(true).toNanoseconds();}

	//! \brief Returns microseconds representation of current system time.
	static double elapsed_system_u() {return PISystemTime::current(true).toMicroseconds();}

	//! \brief Returns milliseconds representation of current system time.
	static double elapsed_system_m() {return PISystemTime::current(true).toMilliseconds();}

	//! \brief Returns seconds representation of current system time.
	static double elapsed_system_s() {return PISystemTime::current(true).toSeconds();}

	//! \brief Returns time mark of current system time.
	static PISystemTime elapsed_system() {return PISystemTime::current(true);}
	
private:
	PISystemTime t_st, t_cur;
	
};

#endif // PITIME_H
