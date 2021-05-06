#ifndef KX_PROTOCOL_C_H
#define KX_PROTOCOL_C_H

#include "piprotocol.h"


#pragma pack (push, 1)


struct KX_C_Header {
	uchar type;
	uchar addr_to;
};



struct KX_C_Command: KX_C_Header {
	KX_C_Command() {command = -1;}
	int command;
	uint checksum;
};


struct KX_C_Event: KX_C_Header {
	KX_C_Event() {event = -1;}
	PISystemTime time;
	int event;
	uint checksum;
};


#pragma pack (pop)


// Client side

class KX_Protocol_C: public PIProtocol
{
	PIOBJECT_SUBCLASS(KX_Protocol_C, PIProtocol)
public:
	KX_Protocol_C(const PIString & config, const PIString & name = "c"): PIProtocol(config, name, &from_buff, 2, &(from_buff.command), sizeof(from_buff) - 2, &to_pult, sizeof(to_pult)) {
		PIConfig conf(config, PIIODevice::ReadOnly);
		PIConfig::Entry ce = conf.getValue(name);
		to_pult.type = from_buff.type = ce.getValue("type", 0xC);
		to_pult.addr_to = ce.getValue("addr_pult", 0x15);
		from_buff.addr_to = ce.getValue("addr_c", 0x75);
		from_pult = from_buff;
		packetExtractor()->setSplitMode(PIPacketExtractor::Header);
		packetExtractor()->setHeader(PIByteArray(&from_buff, 2));
		startReceive();
	}

	void sendEvent(int ev) {
		to_pult.time = PISystemTime::current();
		to_pult.event = ev;
		send();
	}
	EVENT1(commandReceived, int, command)

	KX_C_Command from_pult;
	KX_C_Event to_pult;

private:
	bool validate() {
		if (checksum_i(&from_buff, sizeof(from_buff) - 4) != from_buff.checksum) return false;
		from_pult = from_buff;
		commandReceived(from_pult.command);
		return true;
	}
	bool aboutSend() {
		to_pult.checksum = checksum_i(&to_pult, sizeof(to_pult) - 4);
		return true;
	}
	
	KX_C_Command from_buff;
	
};



// Pult side

class __KX_Protocol_C: public PIProtocol
{
	PIOBJECT_SUBCLASS(__KX_Protocol_C, PIProtocol)
public:
	__KX_Protocol_C(const PIString & config, const PIString & name): PIProtocol(config, name, &from_buff, 2, &(from_buff.time), sizeof(from_buff) - 2, &to_c, sizeof(to_c)) {
		PIConfig conf(config, PIIODevice::ReadOnly);
		PIConfig::Entry ce = conf.getValue(name);
		to_c.type = from_buff.type = ce.getValue("type", 0xC);
		to_c.addr_to = ce.getValue("addr_c", 0x75);
		from_buff.addr_to = ce.getValue("addr_pult", 0x15);
		from_c = from_buff;
		packetExtractor()->setSplitMode(PIPacketExtractor::Header);
		packetExtractor()->setHeader(PIByteArray(&from_buff, 2));
		startReceive();
	}

	void sendCommand(int cmd) {
		to_c.command = cmd;
		send();
	}
	EVENT2(eventReceived, int, event, PISystemTime, time)

	KX_C_Event from_c;
	KX_C_Command to_c;

private:
	bool validate() {
		if (checksum_i(&from_buff, sizeof(from_buff) - 4) != from_buff.checksum) return false;
		from_c = from_buff;
		eventReceived(from_c.event, from_c.time);
		return true;
	}
	bool aboutSend() {
		//piCout << "send command" << to_c.command;
		to_c.checksum = checksum_i(&to_c, sizeof(to_c) - 4);
		return true;
	}
	
	KX_C_Event from_buff;
	
};


#endif // KX_PROTOCOL_C_H
