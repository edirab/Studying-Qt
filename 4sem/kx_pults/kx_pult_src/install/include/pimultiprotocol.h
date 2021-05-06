/*
    PIP - Platform Independent Primitives
    Multiprotocol
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

#ifndef PIMULTIPROTOCOL_H
#define PIMULTIPROTOCOL_H

#include "piprotocol.h"

class PIMultiProtocol: public PIMultiProtocolBase /// DEPRECATED
{
public:
	PIMultiProtocol() {;} /// DEPRECATED
	virtual ~PIMultiProtocol() {clear();}

	void addProtocol(PIProtocol & prot) {prots.push_back(&prot); prot.setMultiProtocolOwner(this); prot.new_mp_prot = false;}
	void addProtocol(PIProtocol * prot) {prots.push_back(prot); prot->setMultiProtocolOwner(this); prot->new_mp_prot = false;}
	void addProtocol(const PIString & config, const PIString & name, void * recHeaderPtr = 0, int recHeaderSize = 0,
					 void * recDataPtr = 0, int recDataSize = 0, void * sendDataPtr = 0, int sendDataSize = 0) {;
		prots.push_back(new PIProtocol(config, name, recHeaderPtr, recHeaderSize, recDataPtr, recDataSize, sendDataPtr, sendDataSize));
		prots.back()->setMultiProtocolOwner(this);
		prots.back()->new_mp_prot = true;
	}
	PIProtocol * protocol(const PIString & name) {piForeach (PIProtocol * i, prots) if (i->name() == name) return i; return 0;}
	PIProtocol * protocol(const int index) {return prots[index];}
	PIProtocol * operator [](const int index) {return prots[index];}

	void startSend() {piForeach (PIProtocol * i, prots) i->startSend();}
	void startReceive() {piForeach (PIProtocol * i, prots) i->startReceive();}
	void start() {piForeach (PIProtocol * i, prots) i->start();}

	void stopSend() {piForeach (PIProtocol * i, prots) i->stopSend();}
	void stopReceive() {piForeach (PIProtocol * i, prots) i->stopReceive();}
	void stop() {piForeach (PIProtocol * i, prots) i->stop();}

	PIProtocol::Quality worseQuality() const {PIProtocol::Quality cq = PIProtocol::Good; piForeachC (PIProtocol * i, prots) if (cq > i->quality()) cq = i->quality(); return cq;}
	PIProtocol::Quality bestQuality() const {PIProtocol::Quality cq = PIProtocol::Unknown; piForeachC (PIProtocol * i, prots) if (cq < i->quality()) cq = i->quality(); return cq;}

	int count() const {return prots.size_s();}
	void clear() {stop(); piForeach (PIProtocol * i, prots) if (i->new_mp_prot) delete i; prots.clear();}

private:
	PIVector<PIProtocol * > prots;

};

class PIRepeater: public PIMultiProtocol { /// DEPRECATED
public:
	PIRepeater(const PIString & config, const PIString & name_) { /// DEPRECATED
		PIConfig conf(config, PIIODevice::ReadOnly);
		if (!conf.isOpened()) {
			piCoutObj << "[PIRepeater \"" << name_ << "\"] Can`t open \"" << config << "\"!";
			return;
		}
		PIConfig::Entry & b(conf.getValue(name_));
		if (b.childCount() != 2) {
			piCoutObj << "[PIRepeater \"" << name_ << "\"] \"" << config << "\" should consist 2 nodes!";
			return;
		}
		addProtocol(config, b.child(0)->fullName());
		addProtocol(config, b.child(1)->fullName());
		start();
	}

	PIString firstChannelName() {if (count() == 2) return protocol(0)->receiverDeviceName() + " -> " + protocol(1)->senderDeviceName(); return "Config error";}
	PIString secondChannelName() {if (count() == 2) return protocol(1)->receiverDeviceName() + " -> " + protocol(0)->senderDeviceName(); return "Config error";}

	ullong receiveCount() {if (count() == 2) return protocol(0)->receiveCount(); return 0;}
	const ullong * receiveCount_ptr() {if (count() == 2) return protocol(0)->receiveCount_ptr(); return 0;}
	ullong sendCount() {if (count() == 2) return protocol(0)->sendCount(); return 0;}
	const ullong * sendCount_ptr() {if (count() == 2) return protocol(0)->sendCount_ptr(); return 0;}

private:
	void received(PIProtocol * prot, bool , uchar * data, int size) {if (prot == protocol(0)) protocol(1)->send(data, size); else protocol(0)->send(data, size);}

};

#endif // PIMULTIPROTOCOL_H
