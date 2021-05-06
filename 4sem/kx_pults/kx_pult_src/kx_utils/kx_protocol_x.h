#ifndef KX_PROTOCOL_X_H
#define KX_PROTOCOL_X_H

#include "piprotocol.h"

#define KX_X_PACKET_NUM  20
#define KX_X_COUNT       16384

extern double X[KX_X_COUNT];


#pragma pack (push, 1)


struct KX_X_Header {
	uchar type;
	uchar addr_to;
};



struct KX_X_Numbers: KX_X_Header {
	int x_num[KX_X_PACKET_NUM];
	uint checksum;
};


struct KX_X_Data: KX_X_Header {
	int x_num[KX_X_PACKET_NUM];
	double x_data[KX_X_PACKET_NUM];
	uint checksum;
};


#pragma pack (pop)


// Client side

class KX_Protocol_X: public PIProtocol
{
	PIOBJECT_SUBCLASS(KX_Protocol_X, PIProtocol)
public:
	KX_Protocol_X(const PIString & config, const PIString & name = "x"): PIProtocol(config, name, &from_buff, 2, &(from_buff.x_num[0]), sizeof(from_buff) - 2, &to_pult, sizeof(to_pult)) {
		PIConfig conf(config, PIIODevice::ReadOnly);
		PIConfig::Entry ce = conf.getValue(name);
		to_pult.type = from_buff.type = ce.getValue("type", 0xB);
		to_pult.addr_to = ce.getValue("addr_pult", 0x15);
		from_buff.addr_to = ce.getValue("addr_x", 0x75);
		from_pult = from_buff;
		for (int i = 0; i < KX_X_PACKET_NUM; ++i)
			to_pult.x_num[i] = from_pult.x_num[i] = -1;
		packetExtractor()->setSplitMode(PIPacketExtractor::Header);
		packetExtractor()->setHeader(PIByteArray(&from_buff, 2));
		start();
	}

	KX_X_Numbers from_pult;
	KX_X_Data to_pult;

private:
	bool validate() {
		if (checksum_i(&from_buff, sizeof(from_buff) - 4) != from_buff.checksum) return false;
		from_pult = from_buff;
		return true;
	}
	bool aboutSend() {
		for (int i = 0; i < KX_X_PACKET_NUM; ++i) {
			to_pult.x_num[i] = from_pult.x_num[i];
			to_pult.x_data[i] = from_pult.x_num[i] < 0 ? 0. : X[from_pult.x_num[i]];
		}
		to_pult.checksum = checksum_i(&to_pult, sizeof(to_pult) - 4);
		return true;
	}
	
	KX_X_Numbers from_buff;
	
};



// Pult side

class __KX_Protocol_X: public PIProtocol
{
	PIOBJECT_SUBCLASS(__KX_Protocol_X, PIProtocol)
public:
	__KX_Protocol_X(const PIString & config, const PIString & name): PIProtocol(config, name, &from_buff, 2, &(from_buff.x_num[0]), sizeof(from_buff) - 2, &to_x, sizeof(to_x)) {
		PIConfig conf(config, PIIODevice::ReadOnly);
		PIConfig::Entry ce = conf.getValue(name);
		to_x.type = from_buff.type = ce.getValue("type", 0xB);
		to_x.addr_to = ce.getValue("addr_x", 0x75);
		from_buff.addr_to = ce.getValue("addr_pult", 0x15);
		from_x = from_buff;
		for (int i = 0; i < KX_X_PACKET_NUM; ++i)
			to_x.x_num[i] = -1;
		packetExtractor()->setSplitMode(PIPacketExtractor::Header);
		packetExtractor()->setHeader(PIByteArray(&from_buff, 2));
		start();
	}

	KX_X_Data from_x;
	KX_X_Numbers to_x;

private:
	bool validate() {
		if (checksum_i(&from_buff, sizeof(from_buff) - 4) != from_buff.checksum) return false;
		from_x = from_buff;
		return true;
	}
	bool aboutSend() {
		to_x.checksum = checksum_i(&to_x, sizeof(to_x) - 4);
		return true;
	}
	
	KX_X_Data from_buff;
	
};


#endif // KX_PROTOCOL_X_H
