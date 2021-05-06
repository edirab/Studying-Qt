/*! \file pipeer.h
 * \brief Peering net node
*/
/*
    PIP - Platform Independent Primitives
    Peer - named I/O ethernet node, forming self-organized peering network
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

#ifndef PIPEER_H
#define PIPEER_H

#include "piethernet.h"
#include "pidiagnostics.h"
#include "pidatatransfer.h"

class PIP_EXPORT PIPeer: public PIIODevice
{
	PIIODEVICE(PIPeer)
private:
	
	class PeerData: public PIObject {
		PIOBJECT_SUBCLASS(PeerData, PIObject)
	public:
		PeerData(const PIString & n);
		~PeerData();
		EVENT_HANDLER1(void, dtSendRequestIn, PIByteArray &, data) {data.push_front(uchar(2)); sendRequest(name(), data);}
		EVENT_HANDLER1(void, dtSendRequestOut, PIByteArray &, data) {data.push_front(uchar(3)); sendRequest(name(), data);}
		EVENT_HANDLER1(void, dtReceiveFinishedIn, bool, ok) {if (ok) received(name(), dt_in.data());}
		EVENT_HANDLER1(void, dtReceiveFinishedOut, bool, ok) {if (ok) received(name(), dt_out.data());}
		EVENT_HANDLER(void, dtThread);
		EVENT2(received, const PIString &, from, const PIByteArray &, data)
		EVENT2(sendRequest, const PIString &, to, const PIByteArray &, data)
		bool send(const PIByteArray & d);
		void receivedPacket(uchar type, const PIByteArray & d);
		void setDist(int dist);
		PIByteArray data;
		PIThread t;
		PIDataTransfer dt_in, dt_out;
	};
	
public:
	explicit PIPeer(const PIString & name = PIString());
	virtual ~PIPeer();

	class PeerInfo {
		friend class PIPeer;
		friend PIByteArray & operator <<(PIByteArray & s, const PIPeer::PeerInfo & v);
		friend PIByteArray & operator >>(PIByteArray & s, PIPeer::PeerInfo & v);
	public:
		PeerInfo() {dist = sync = cnt = 0; trace = -1; was_update = false; _data = 0;}
		~PeerInfo() {}
		
		struct Address {
			Address(const PIString & a = PIString(), const PIString & m = "255.255.255.0");
			bool isAvailable() const {return ping > 0;}
			//inline const Address & operator =(const Address & v) {address = v.address; netmask = v.netmask; piCout << "!!!!!!!!!" << last_ping; return *this;}
			PIString address;
			PIString netmask;
			double ping; // ms
			bool wait_ping;
			PISystemTime last_ping;
		};
		
		PIString name;
		PIVector<Address> addresses;
		int dist;
		PIStringList neighbours;
		
		bool isNeighbour() const {return dist == 0;}
		int ping() const;
		PIString fastestAddress() const;
		
	protected:
		void addNeighbour(const PIString & n) {if (!neighbours.contains(n)) neighbours << n;}
		void addNeighbours(const PIStringList & l) {piForeachC (PIString & n, l) if (!neighbours.contains(n)) neighbours << n;}
		void removeNeighbour(const PIString & n) {neighbours.removeAll(n);}
		void resetPing() {for (int i = 0; i < addresses.size_s(); ++i) addresses[i].ping = -1;}
		void init();
		void destroy();
		
		int sync, cnt, trace;
		bool was_update;
		PISystemTime time;
		PeerData * _data;
		
	};
	
	friend PIByteArray & operator <<(PIByteArray & s, const PIPeer::PeerInfo & v);
	friend PIByteArray & operator >>(PIByteArray & s, PIPeer::PeerInfo & v);

	bool send(const PIString & to, const PIByteArray & data) {return send(to, data.data(), data.size_s());}
	bool send(const PIString & to, const PIString & data) {return send(to, data.data(), data.size_s());}
	bool send(const PIString & to, const void * data, int size);
	bool send(const PeerInfo & to, const PIByteArray & data) {return send(to.name, data.data(), data.size_s());}
	bool send(const PeerInfo & to, const PIString & data) {return send(to.name, data.data(), data.size_s());}
	bool send(const PeerInfo & to, const void * data, int size) {return send(to.name, data, size);}
	bool send(const PeerInfo * to, const PIByteArray & data) {if (to == 0) return false; return send(to->name, data.data(), data.size_s());}
	bool send(const PeerInfo * to, const PIString & data) {if (to == 0) return false; return send(to->name, data.data(), data.size_s());}
	bool send(const PeerInfo * to, const void * data, int size) {if (to == 0) return false; return send(to->name, data, size);}
	void sendToAll(const PIByteArray & data) {piForeachC (PeerInfo & i, peers) send(i.name, data.data(), data.size_s());}
	void sendToAll(const PIString & data) {piForeachC (PeerInfo & i, peers) send(i.name, data.data(), data.size_s());}
	void sendToAll(const void * data, int size) {piForeachC (PeerInfo & i, peers) send(i.name, data, size);}
	
	bool isMulticastReceive() const {return !eths_mcast.isEmpty();}
	bool isBroadcastReceive() const {return !eths_bcast.isEmpty();}
	
	PIDiagnostics & diagnosticService() {return diag_s;}
	PIDiagnostics & diagnosticData() {return diag_d;}
	
	const PIVector<PIPeer::PeerInfo> & allPeers() const {return peers;}
	bool isPeerExists(const PIString & name) const {return getPeerByName(name) != 0;}
	
	const PeerInfo * getPeerByName(const PIString & name) const {return peers_map.value(name, 0);}
	const PeerInfo & selfInfo() const {return self_info;}
	const PIMap<PIString, PIVector<PeerInfo * > >  & _peerMap() const {return addresses_map;}
	
	void reinit();
	void lock() {peers_mutex.lock();}
	void unlock() {peers_mutex.unlock();}
	void changeName(const PIString & new_name);
	const PIString & trustPeerName() const {return trust_peer;}
	void setTrustPeerName(const PIString & peer_name) {trust_peer = peer_name;}
	void setTcpServerIP(const PIString & ip) {server_ip = ip; tcpClientReconnect();}

	
	EVENT2(dataReceivedEvent, const PIString &, from, const PIByteArray &, data)
	EVENT1(peerConnectedEvent, const PIString &, name)
	EVENT1(peerDisconnectedEvent, const PIString &, name)
	
protected:
	virtual void dataReceived(const PIString & from, const PIByteArray & data) {;}
	virtual void peerConnected(const PIString & name) {;}
	virtual void peerDisconnected(const PIString & name) {;}

	EVENT_HANDLER2(bool, dataRead, uchar *, readed, int, size);
	EVENT_HANDLER2(bool, mbcastRead, uchar *, readed, int, size);
	
private:
	EVENT_HANDLER2(void, timerEvent, void * , data, int, delim);
	EVENT_HANDLER2(bool, sendInternal, const PIString &, to, const PIByteArray &, data);
	EVENT_HANDLER2(void, dtReceived, const PIString &, from, const PIByteArray &, data);
	EVENT_HANDLER1(void, newTcpClient, PIEthernet * , client);
	EVENT_HANDLER(void, tcpClientReconnect);

	bool hasPeer(const PIString & name) {piForeachC (PeerInfo & i, peers) if (i.name == name) return true; return false;}
	bool removePeer(const PIString & name);
	void removeNeighbour(const PIString & name);
	void addPeer(const PeerInfo & pd);

	void sendPeerInfo(const PeerInfo & info);
	void sendPeerRemove(const PIString & peer);
	void sendSelfInfo() {sendPeerInfo(self_info);}
	void sendSelfRemove() {sendPeerRemove(self_info.name);}
	void syncPeers();
	void checkNetwork();
	void initNetwork();
	void buildMap();
	void initEths(PIStringList al);
	void initMBcasts(PIStringList al);
	void destroyEths();
	void destroyMBcasts();
	void sendMBcast(const PIByteArray & ba);
	void pingNeighbours();
	void addToRemoved(const PeerInfo & pi) {removed[pi.name] = PIPair<int, PISystemTime>(pi.cnt, pi.time);}
	bool isRemoved(const PeerInfo & pi) const {return (removed.value(pi.name) == PIPair<int, PISystemTime>(pi.cnt, pi.time));}

	bool openDevice();
	bool closeDevice();
	PIString fullPathPrefix() const {return "peer";}
	PIString constructFullPathDevice() const;
	void configureFromFullPathDevice(const PIString &full_path);
	int readDevice(void * read_to, int max_size);
	int writeDevice(const void * data, int size);


	PeerInfo * quickestPeer(const PIString & to);
	bool sendToNeighbour(PeerInfo * peer, const PIByteArray & ba);
	inline static bool isPeerRecent(const PeerInfo & my, const PeerInfo & income) {return (my.cnt < income.cnt) || (my.time < income.time);}

	// 1 - new peer, 2 - remove peer, 3 - sync peers, 4 - data, 5 - ping request, 6 - ping reply
	// Data packet: 4, from, to, ticks, data_size, data

protected:
	PIMutex mc_mutex, eth_mutex, peers_mutex, send_mutex, send_mc_mutex;

private:
	PIVector<PIEthernet * > eths_traffic, eths_mcast, eths_bcast;
	PIEthernet::InterfaceList prev_ifaces;
	PIEthernet eth_send, eth_lo, eth_tcp_srv, eth_tcp_cli;
	PITimer sync_timer;
	PeerInfo self_info;
	PIVector<PeerInfo> peers;
	PIMap<PIString, PeerInfo * > peers_map;
	PIMap<PIString, PIVector<PeerInfo * > > addresses_map; // map {"to" = list of nearest peers}
	PIMap<PIString, PIPair<int, PISystemTime> > removed;
	PIDiagnostics diag_s, diag_d;
	bool destroyed, no_timer;
	PIString trust_peer;
	PIString server_ip;
	PIMutex read_buffer_mutex;
	PIQueue<PIByteArray> read_buffer;
	int read_buffer_size;
	};

inline PICout operator <<(PICout c, const PIPeer::PeerInfo::Address & v) {c.space(); c << "PeerAddress(" << v.address << ", " << v.netmask << ", " << v.ping << ")"; return c;}
inline PICout operator <<(PICout c, const PIPeer::PeerInfo & v) {c.space(); c << "PeerInfo(" << v.name << ", " << v.dist << ", " << v.addresses << ")"; return c;}

inline PIByteArray & operator <<(PIByteArray & s, const PIPeer::PeerInfo::Address & v) {s << v.address << v.netmask << v.ping; return s;}
inline PIByteArray & operator >>(PIByteArray & s, PIPeer::PeerInfo::Address & v) {s >> v.address >> v.netmask >> v.ping; return s;}

inline PIByteArray & operator <<(PIByteArray & s, const PIPeer::PeerInfo & v) {s << v.name << v.addresses << v.dist << v.neighbours << v.cnt << v.time; return s;}
inline PIByteArray & operator >>(PIByteArray & s, PIPeer::PeerInfo & v) {s >> v.name >> v.addresses >> v.dist >> v.neighbours >> v.cnt >> v.time; return s;}

#endif // PIPEER_H
