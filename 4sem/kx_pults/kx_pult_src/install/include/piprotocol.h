/*! \file piprotocol.h
 * \brief Highly configurable from file I/O channel
*/
/*
    PIP - Platform Independent Primitives
    Protocol, input/output channel (COM, UDP)
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

#ifndef PIPROTOCOL_H
#define PIPROTOCOL_H

#include "piserial.h"
#include "piethernet.h"
#include "pipacketextractor.h"
#include "pitimer.h"
#include "piconfig.h"
#include "pifile.h"
#include "math.h"

class PIProtocol; /// DEPRECATED

class PIP_EXPORT PIMultiProtocolBase: protected PIObject  /// DEPRECATED
{
	PIOBJECT_SUBCLASS(PIMultiProtocolBase, PIObject)
	friend class PIProtocol;
public:
	PIMultiProtocolBase() {;} /// DEPRECATED
	virtual ~PIMultiProtocolBase() {;}

protected:
	virtual void received(PIProtocol * prot, bool corrected, uchar * data, int size) {;}

private:
	static void receiveEvent(PIMultiProtocolBase * p, PIProtocol * prot, bool corrected, uchar * data, int size) {p->mutex_receive.lock(); p->received(prot, corrected, data, size); p->mutex_receive.unlock();}

	PIMutex mutex_receive;

};

typedef void (*ReceiveFunc)(void * );

/// events:
/// void receiverStarted()
/// void receiverStopped()
/// void senderStarted()
/// void senderStopped()
/// void received(bool validate_is_ok)
/// void qualityChanged(PIProtocol::Quality old_quality, PIProtocol::Quality new_quality)
///
/// handlers:
/// void startReceive(float exp_frequency = -1.f)
/// void stopReceive()
/// void startSend(float frequency = -1.f)
/// void stopSend()
/// void start()
/// void stop()
/// void send()
/// void send(const void * data, int size, bool direct = false)
class PIP_EXPORT PIProtocol: public PIObject /// DEPRECATED
{
	PIOBJECT_SUBCLASS(PIProtocol, PIObject)
	friend class PIMultiProtocolBase;
	friend class PIMultiProtocol;
	enum Type {None, Serial, Ethernet};
public:
	
	//! Contructs an empty unconfigured protocol
	PIProtocol(): PIObject() {init();} /// DEPRECATED
	
	//! Contructs protocol configured from file "config", config file section "name"
	PIProtocol(const PIString & config, const PIString & name, void * recHeaderPtr = 0, int recHeaderSize = 0,
			   void * recDataPtr = 0, int recDataSize = 0, void * sendDataPtr = 0, int sendDataSize = 0); // from config
	
	virtual ~PIProtocol();
	
	//! Connection quality
	enum Quality {
		Unknown /** Unknown, no one packet received yet */ = 1,
		Failure /** No connection, no one correct packet received for last period */ = 2,
		Bad /** Bad connection, correct packets received <= 20% */ = 3,
		Average /** Average connection, correct packets received > 20% and <= 80% */ = 4,
		Good /** Good connection, correct packets received > 80% */ = 5
	};
	
	EVENT_HANDLER0(void, startReceive) {startReceive(-1.f);}
	EVENT_HANDLER1(void, startReceive, float, exp_frequency); // if "frequency = -1" used last passed value
	EVENT_HANDLER0(void, stopReceive);
	void setExpectedFrequency(float frequency); // for connection quality diagnostic
	void setReceiverDevice(const PIString & device, PISerial::Speed speed, bool force = false); // for Serial
	void setReceiverData(void * dataPtr, int dataSize) {this->dataPtr = (uchar * )dataPtr; this->dataSize = dataSize; packet_ext->setHeader(PIByteArray(headerPtr, headerSize)); packet_ext->setPayloadSize(dataSize); packet_ext->setPacketSize(dataSize);}
	void setReceiverDataHeader(void * headerPtr, int headerSize) {this->headerPtr = (uchar * )headerPtr; this->headerSize = headerSize; packet_ext->setHeader(PIByteArray(headerPtr, headerSize)); packet_ext->setPayloadSize(dataSize); packet_ext->setPacketSize(dataSize);}
	void setReceiverAddress(const PIString & ip, int port, bool force = false); // for Ethernet
	void setReceiverParameters(PIFlags<PISerial::Parameters> parameters) {if (type_rec == PIProtocol::Serial || type_send == PIProtocol::Serial) ser->setParameters(parameters);} // for Serial
	void setReceiveSlot(ReceiveFunc slot) {ret_func = slot;}
	float expectedFrequency() const {return exp_freq;}

	EVENT_HANDLER0(void, startSend) {startSend(-1.f);} // if "frequency = -1" used last passed value
	EVENT_HANDLER1(void, startSend, float, frequency); // if "frequency = -1" used last passed value
	EVENT_HANDLER0(void, stopSend) {sendTimer->stop(); senderStopped();}
	void setSenderFrequency(float frequency) {send_freq = frequency;}
	void setSenderDevice(const PIString & device, PISerial::Speed speed, bool force = false); // for Serial
	void setSenderData(void * dataPtr, int dataSize) {sendDataPtr = (uchar * )dataPtr; sendDataSize = dataSize;}
	void setSenderAddress(const PIString & ip, int port, bool force = false); // for Ethernet
	void setSenderIP(const PIString & ip, bool force = false); // for Ethernet
	void setSenderPort(int port, bool force = false); // for Ethernet
	void setSenderParameters(PIFlags<PISerial::Parameters> parameters) {if (type_send == PIProtocol::Serial) ser->setParameters(parameters);} // for Serial
	float senderFrequency() const {return send_freq;}

	EVENT_HANDLER0(void, start) {startReceive(); startSend();}
	EVENT_HANDLER0(void, stop) {stopReceive(); stopSend();}
	EVENT_HANDLER0(void, send);
	EVENT_HANDLER2(void, send, const void *, data, int, size) {send(data, size, false);}
	EVENT_HANDLER3(void, send, const void *, data, int, size, bool, direct);

	void setName(const PIString & name) {protName = name; PIObject::setName(name);}
	PIString name() const {return protName;}
	void setDisconnectTimeout(float timeout) {timeout_ = timeout; changeDisconnectTimeout();}
	float disconnectTimeout() const {return timeout_;}
	const float * disconnectTimeout_ptr() const {return &timeout_;}
	float immediateFrequency() const {return immediate_freq;}
	float integralFrequency() const {return integral_freq;}
	const float * immediateFrequency_ptr() const {return &immediate_freq;}
	const float * integralFrequency_ptr() const {return &integral_freq;}
	ullong receiveCountPerSec() const {return packets_in_sec;}
	const ullong * receiveCountPerSec_ptr() const {return &packets_in_sec;}
	ullong sendCountPerSec() const {return packets_out_sec;}
	const ullong * sendCountPerSec_ptr() const {return &packets_out_sec;}
	ullong receiveBytesPerSec() const {return bytes_in_sec;}
	const ullong * receiveBytesPerSec_ptr() const {return &bytes_in_sec;}
	ullong sendBytesPerSec() const {return bytes_out_sec;}
	const ullong * sendBytesPerSec_ptr() const {return &bytes_out_sec;}
	ullong receiveCount() const {return receive_count;}
	const ullong * receiveCount_ptr() const {return &receive_count;}
	ullong wrongCount() const {return wrong_count;}
	const ullong * wrongCount_ptr() const {return &wrong_count;}
	ullong sendCount() const {return send_count;}
	const ullong * sendCount_ptr() const {return &send_count;}
	ullong missedCount() const {return missed_count;}
	const ullong * missedCount_ptr() const {return &missed_count;}
	PIProtocol::Quality quality() const {return net_diag;} // receive quality
	const int * quality_ptr() const {return (int * )&net_diag;} // receive quality pointer
	PIString receiverDeviceName() const {return devReceiverName;}
	PIString senderDeviceName() const {return devSenderName;}
	PIString receiverDeviceState() const {return devReceiverState;}
	const PIString * receiverDeviceState_ptr() const {return &devReceiverState;}
	PIString senderDeviceState() const {return devSenderState;}
	const PIString * senderDeviceState_ptr() const {return &devSenderState;}
	PIString receiveSpeed() const {return speedIn;}
	const PIString * receiveSpeed_ptr() const {return &speedIn;}
	PIString sendSpeed() const {return speedOut;}
	const PIString * sendSpeed_ptr() const {return &speedOut;}
	PIString receiverHistorySize() const {return history_rsize_rec;}
	const PIString * receiverHistorySize_ptr() const {return &history_rsize_rec;}
	PIString senderHistorySize() const {return history_rsize_send;}
	const PIString * senderHistorySize_ptr() const {return &history_rsize_send;}
	bool writeReceiverHistory() const {return history_write_rec;}
	const bool * writeReceiverHistory_ptr() const {return &history_write_rec;}
	bool writeSenderHistory() const {return history_write_send;}
	const bool * writeSenderHistory_ptr() const {return &history_write_send;}

	void * receiveData() {return dataPtr;}
	void * sendData() {return sendDataPtr;}

	PIPacketExtractor * packetExtractor() {return packet_ext;}
//	PIByteArray lastHeader() {return packet_ext->lastHeader();}

	EVENT0(receiverStarted)
	EVENT0(receiverStopped)
	EVENT0(senderStarted)
	EVENT0(senderStopped)
	EVENT1(received, bool, validate_is_ok)
	EVENT2(qualityChanged, PIProtocol::Quality, new_quality, PIProtocol::Quality, old_quality)

protected:
	virtual bool receive(uchar * data, int size) {if (dataPtr != 0) memcpy(dataPtr, data, size); return true;} // executed when raw data received, break if 'false' return
	virtual bool validate() {return true;} // function for validate algorithm and save data from dataPtr to external struct
	virtual bool headerValidate(uchar * src, uchar * rec, int size) {for (int i = 0; i < size; ++i) if (src[i] != rec[i]) return false; return true;} // function for validate header (COM-port and headerSize > 0)
	virtual uint checksum_i(void * data, int size) { // function for checksum (uint)
		uint c = 0;
		for (int i = 0; i < size; ++i)
			c += ((uchar*)data)[i];
		return ~(c + 1);
	}
	virtual uchar checksum_c(void * data, int size) { // function for checksum (uchar)
		uchar c = 0;
		for (int i = 0; i < size; ++i)
			c += ((uchar*)data)[i];
		return ~(c + 1);
	}
	virtual bool aboutSend() {return true;} // executed before send data, if return 'false' then data is not sending

	void init();
	void init_sender(PIConfig::Entry & b, PIConfig::Entry & sb, const PIString & config);
	void init_receiver(PIConfig::Entry & b, PIConfig::Entry & rb, const PIString & config);
	void check_state();
	void calc_freq();
	void calc_diag();

	PISerial * ser;
	PIEthernet * eth;
	uint dataSize, headerSize, sendDataSize;
	uchar * dataPtr, * headerPtr, * sendDataPtr;

private:
	static void sendEvent(void * e, int) {((PIProtocol * )e)->send();}
	static bool receiveEvent(void * t, uchar * data, int size);
	static bool headerValidateEvent(void * t, uchar * src, uchar * rec, int size) {return ((PIProtocol * )t)->headerValidate(src, rec, size);}
	static void diagEvent(void * t, int);
	static void secEvent(void * t, int);

	void setMultiProtocolOwner(PIMultiProtocolBase * mp) {mp_owner = mp;}
	PIMultiProtocolBase * multiProtocolOwner() const {return mp_owner;}
	void changeDisconnectTimeout();

	ReceiveFunc ret_func;
	PIPacketExtractor * packet_ext;
	PITimer * diagTimer, * sendTimer, * secTimer;
	PITimeMeasurer diag_tm;
	PIMultiProtocolBase * mp_owner;
	PIProtocol::Type type_send, type_rec;
	PIProtocol::Quality net_diag;
	PIDeque<float> last_freq;
	PIDeque<char> last_packets;
	PIString protName, devReceiverName, devReceiverState, devSenderName, devSenderState, speedIn, speedOut;
	PIString history_path_rec, history_path_send, history_rsize_rec, history_rsize_send;
	PIFile history_file_rec, history_file_send;
	ushort history_id_rec, history_id_send;
	bool work, new_mp_prot, history_write_rec, history_write_send;
	float exp_freq, send_freq, ifreq, immediate_freq, integral_freq, timeout_;
	int packets[2], pckt_cnt, pckt_cnt_max;
	char cur_pckt;
	ullong wrong_count, receive_count, send_count, missed_count, packets_in_sec, packets_out_sec, bytes_in_sec, bytes_out_sec;

};

#endif // PIPROTOCOL_H
