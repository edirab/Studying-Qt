/*! \file pidiagnostics.h
 * \brief Connection quality diagnostics
*/
/*
    PIP - Platform Independent Primitives
    Speed and quality in/out diagnostics
    Copyright (C) 2016  Ivan Pelipenko peri4ko@yandex.ru, Bychkov Andrey wapmobil@gmail.com

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

#ifndef PIDIAGNOSTICS_H
#define PIDIAGNOSTICS_H

#include "pitimer.h"


class PIP_EXPORT PIDiagnostics: public PITimer
{
	PIOBJECT_SUBCLASS(PIDiagnostics, PITimer)
	friend class PIConnection;
public:
	
	//! Constructs an empty diagnostics and if "start_" start it
	PIDiagnostics(bool start_ = true);
	
	virtual ~PIDiagnostics() {;}
	
	//! Connection quality
	enum Quality {
		Unknown /** Unknown, no one packet received yet */ = 1,
		Failure /** No connection, no one correct packet received for last period */ = 2,
		Bad /** Bad connection, correct packets received <= 20% */ = 3,
		Average /** Average connection, correct packets received > 20% and <= 80% */ = 4,
		Good /** Good connection, correct packets received > 80% */ = 5
	};
	
	
	//! Returns period of full disconnect in seconds and period of averaging frequency
	float disconnectTimeout() const {return disconn_;}
	
	//! Returns period of full disconnect in seconds and period of averaging frequency
	void setDisconnectTimeout(float s) {setProperty("disconnectTimeout", s);}
	
	//! Returns immediate receive frequency, packets/s
	float immediateFrequency() const {return immediate_freq;}
	
	//! Returns integral receive frequency for \a disconnectTimeout() seconds, packets/s
	float integralFrequency() const {return integral_freq;}
	
	//! Returns correct received packets per second
	ullong receiveCountPerSec() const {return packets_recv_sec;}
	
	//! Returns sended packets per second
	ullong sendCountPerSec() const {return packets_send_sec;}
	
	//! Returns correct received bytes per second
	ullong receiveBytesPerSec() const {return bytes_recv_sec;}
	
	//! Returns sended bytes per second
	ullong sendBytesPerSec() const {return bytes_send_sec;}
	
	//! Returns overall correct received bytes
	ullong receiveBytes() const {return bytes_recv;}
	
	//! Returns overall wrong received bytes
	ullong wrongBytes() const {return bytes_wrong;}
	
	//! Returns overall sended bytes
	ullong sendBytes() const {return bytes_send;}
	
	//! Returns overall correct received packets count
	ullong receiveCount() const {return count_recv;}
	
	//! Returns overall wrong received packets count
	ullong wrongCount() const {return count_wrong;}
	
	//! Returns overall sended packets count
	ullong sendCount() const {return count_send;}
	
	//! Returns connection quality
	PIDiagnostics::Quality quality() const {return qual;}
	
	//! Returns receive speed in format "n {B|kB|MB|GB|TB}/s"
	PIString receiveSpeed() const {return speedRecv;}
	
	//! Returns send speed in format "n {B|kB|MB|GB|TB}/s"
	PIString sendSpeed() const {return speedSend;}
	
	
	//! Returns immediate receive frequency pointer, packets/s. Useful for output to PIConsole
	const float * immediateFrequency_ptr() const {return &immediate_freq;}
	
	//! Returns integral receive frequency pointer for period, packets/s. Useful for output to PIConsole
	const float * integralFrequency_ptr() const {return &integral_freq;}
	
	//! Returns correct received packets per second pointer. Useful for output to PIConsole
	const ullong * receiveCountPerSec_ptr() const {return &packets_recv_sec;}
	
	//! Returns sended packets per second pointer. Useful for output to PIConsole
	const ullong * sendCountPerSec_ptr() const {return &packets_send_sec;}
	
	//! Returns correct received bytes per second pointer. Useful for output to PIConsole
	const ullong * receiveBytesPerSec_ptr() const {return &bytes_recv_sec;}
	
	//! Returns sended bytes per second pointer. Useful for output to PIConsole
	const ullong * sendBytesPerSec_ptr() const {return &bytes_send_sec;}
	
	//! Returns overall correct received bytes pointer. Useful for output to PIConsole
	const ullong * receiveBytes_ptr() const {return &bytes_recv;}
	
	//! Returns overall wrong received bytes pointer. Useful for output to PIConsole
	const ullong * wrongBytes_ptr() const {return &bytes_wrong;}
	
	//! Returns overall sended bytes pointer. Useful for output to PIConsole
	const ullong * sendBytes_ptr() const {return &bytes_send;}
	
	//! Returns overall correct received packets count pointer. Useful for output to PIConsole
	const ullong * receiveCount_ptr() const {return &count_recv;}
	
	//! Returns overall wrong received packets count pointer. Useful for output to PIConsole
	const ullong * wrongCount_ptr() const {return &count_wrong;}
	
	//! Returns overall sended packets count pointer. Useful for output to PIConsole
	const ullong * sendCount_ptr() const {return &count_send;}
	
	//! Returns connection quality pointer. Useful for output to PIConsole
	const int * quality_ptr() const {return (int * )&qual;}
	
	//! Returns receive speed pointer in format "n {B|kB|MB|GB|TB}/s". Useful for output to PIConsole
	const PIString * receiveSpeed_ptr() const {return &speedRecv;}
	
	//! Returns send speed pointer in format "n {B|kB|MB|GB|TB}/s". Useful for output to PIConsole
	const PIString * sendSpeed_ptr() const {return &speedSend;}
	
	EVENT_HANDLER0(void, start) {start(100.); changeDisconnectTimeout(disconn_);}
	EVENT_HANDLER1(void, start, double, msecs) {if (msecs > 0.) {PITimer::start(msecs); changeDisconnectTimeout(disconn_);}}
	EVENT_HANDLER0(void, stop) {PITimer::stop();}
	EVENT_HANDLER0(void, reset);
	
	EVENT_HANDLER1(void, received, int, size) {received(size, true);}
	EVENT_HANDLER2(void, received, int, size, bool, correct);
	EVENT_HANDLER1(void, sended, int, size);
	
	EVENT2(qualityChanged, PIDiagnostics::Quality, new_quality, PIDiagnostics::Quality, old_quality)
	
//! \handlers
//! \{
	
	//! \fn void start(double msecs = 1000.)
	//! \brief Start diagnostics evaluations with period "msecs" milliseconds
	
	//! \fn void reset()
	//! \brief Reset diagnostics counters
	
	//! \fn void received(int size, bool correct = true)
	//! \brief Notify diagnostics about "correct" corected received packet
	
	//! \fn void sended(int size)
	//! \brief Notify diagnostics about sended packet
	
//! \}
//! \events
//! \{

	//! \fn void qualityChanged(PIDiagnostics::Quality new_quality, PIDiagnostics::Quality old_quality)
	//! \brief Raise on change receive quality from "old_quality" to "new_quality"

//! \}
	
private:
	struct Entry {
		Entry() {bytes_ok = bytes_fail = 0; cnt_ok = cnt_fail = 0; empty = true;}
		ullong bytes_ok;
		ullong bytes_fail;
		uint cnt_ok;
		uint cnt_fail;
		bool empty;
	};
	
	void tick(void *, int);
	Entry calcHistory(PIQueue<Entry> & hist, int & cnt);
	void propertyChanged(const PIString &);
	void changeDisconnectTimeout(float disct);

	PIDiagnostics::Quality qual;
	PIString speedRecv, speedSend;
	float immediate_freq, integral_freq;
	PIQueue<Entry> history_rec, history_send;
	float disconn_;
	ullong count_wrong, count_recv, count_send, bytes_wrong, bytes_recv, bytes_send;
	ullong packets_recv_sec, packets_send_sec, bytes_recv_sec, bytes_send_sec;
	
};

#endif // PIDIAGNOSTICS_H
