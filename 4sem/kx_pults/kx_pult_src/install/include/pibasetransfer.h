/*! \file pibasetransfer.h
 * \brief Base class for reliable send and receive data in fixed packets with error correction, pause and resume
*/
/*
	PIP - Platform Independent Primitives
	Base class for reliable send and receive data in fixed packets with error correction, pause and resume
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

#ifndef PIBASETRANSFER_H
#define PIBASETRANSFER_H

#include "picrc.h"
#include "pitimer.h"
#include "pidiagnostics.h"

class PIBaseTransfer: public PIObject
{
	PIOBJECT_SUBCLASS(PIBaseTransfer, PIObject)
public:
	PIBaseTransfer();
	~PIBaseTransfer();

# pragma pack(push,1)
	struct PacketHeader {
		uint sig;
		int type; // PacketType
		int session_id;
		uint id;
		uint crc;
		bool check_sig() {return (sig == signature);}
	};
	
	struct Part {
		Part(uint id_ = 0, ullong size_ = 0, ullong start_ = 0) : id(id_), size(size_), start(start_) {}
		uint id;
		ullong size;
		ullong start;
	};
# pragma pack(pop)

	void stopSend();
	void stopReceive();

	bool isSending() const {return is_sending;}
	bool isReceiving() const {return is_receiving;}
	bool isPause() const {return is_pause;}
	void setPause(bool pause_);

	void setPacketSize(int size) {packet_size = size;}
	int packetSize() const {return packet_size;}

	void setTimeout(double sec) {timeout_ = sec; diag.setDisconnectTimeout(sec);}
	double timeout() const {return timeout_;}

	const PIString & stateString() const {return state_string;}
	llong bytesAll() const {return bytes_all;}
	llong bytesCur() const {return bytes_cur;}
	const PIString * stateString_ptr() const {return &state_string;}
	const llong * bytesAll_ptr() const {return &bytes_all;}
	const llong * bytesCur_ptr() const {return &bytes_cur;}
	const PIDiagnostics &diagnostic() {return diag;}

	static uint packetSignature() {return signature;}

	EVENT_HANDLER1(void, received, PIByteArray, data);
	EVENT_HANDLER(void, stop) {stopSend(); stopReceive();}
	EVENT_HANDLER(void, pause) {setPause(true);}
	EVENT_HANDLER(void, resume) {setPause(false);}
	
	EVENT(receiveStarted)
	EVENT(paused)
	EVENT(resumed)
	EVENT1(receiveFinished, bool, ok)
	EVENT(sendStarted)
	EVENT1(sendFinished, bool, ok)
	EVENT1(sendRequest, PIByteArray &, data)

protected:
	uint packet_header_size, part_header_size;
	bool break_, is_sending, is_receiving, is_pause;
	PIString state_string;
	llong bytes_all, bytes_cur;
	
	void buildSession(PIVector<Part> parts);
	virtual PIByteArray buildPacket(Part fi) = 0;
    virtual void receivePart(Part fi, PIByteArray ba, PIByteArray pheader) = 0;
	virtual PIByteArray customHeader() {return PIByteArray();}
	bool send_process();
	
private:

	enum PacketType {pt_Unknown, pt_Data, pt_ReplySuccess, pt_ReplyInvalid, pt_Break, pt_Start, pt_Pause};

# pragma pack(push,1)
	struct StartRequest {
		uint packets;
		ullong size;
	};
# pragma pack(pop)
	
	friend PIByteArray & operator >>(PIByteArray & s, PIBaseTransfer::StartRequest & v);
	friend PIByteArray & operator <<(PIByteArray & s, const PIBaseTransfer::StartRequest & v);

	static const uint signature;
	int packet_size, packets_count;
	double timeout_;
	PIVector<PIVector<Part> > session;
	PIVector<PacketType> replies;
	PITimeMeasurer send_tm, pause_tm;
	PacketHeader header;
	CRC_16 crc;
	int send_queue;
	PIDiagnostics diag;
	PIMutex mutex_session;
	PIMutex mutex_send;
	PIMutex mutex_header;

	void processData(int id, PIByteArray &data);
	PIByteArray build_packet(int id);
	int checkSession();
	void sendBreak(int session_id);
	void sendReply(PacketType reply);
	bool getStartRequest();
	bool finish_send(bool ok);
	void finish_receive(bool ok, bool quet = false);
};

inline PIByteArray & operator <<(PIByteArray & s, const PIBaseTransfer::PacketHeader & v) {s << v.sig << v.type << v.session_id << v.id << v.crc; return s;}
inline PIByteArray & operator >>(PIByteArray & s, PIBaseTransfer::PacketHeader & v) {s >> v.sig >> v.type >> v.session_id >> v.id >> v.crc; return s;}

inline PIByteArray & operator <<(PIByteArray & s, const PIBaseTransfer::Part & v) {s << v.id << v.size << v.start; return s;}
inline PIByteArray & operator >>(PIByteArray & s, PIBaseTransfer::Part & v) {s >> v.id >> v.size >> v.start; return s;}

inline PIByteArray & operator <<(PIByteArray & s, const PIBaseTransfer::StartRequest & v) {s << v.packets << v.size; return s;}
inline PIByteArray & operator >>(PIByteArray & s, PIBaseTransfer::StartRequest & v) {s >> v.packets >> v.size; return s;}

inline PICout operator <<(PICout s, const PIBaseTransfer::Part & v) {s.setControl(0, true); s << "Part(\"" << v.id  << "\", " << PIString::readableSize(v.start) << " b | " << PIString::readableSize(v.size) << " b)"; s.restoreControl(); return s;}


#endif // PIBASETRANSFER_H
