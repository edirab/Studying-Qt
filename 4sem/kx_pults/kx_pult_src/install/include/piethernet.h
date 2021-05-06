/*! \file piethernet.h
 * \brief Ethernet device
*/
/*
    PIP - Platform Independent Primitives
    Ethernet, UDP/TCP Broadcast/Multicast
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

#ifndef PIETHERNET_H
#define PIETHERNET_H

#include "pitimer.h"
#include "piiodevice.h"

class sockaddr;

class PIP_EXPORT PIEthernet: public PIIODevice
{
	PIIODEVICE(PIEthernet)
	friend class PIPeer;
public:
	
	//! Contructs UDP %PIEthernet with empty read address
	explicit PIEthernet();

	//! \brief Type of %PIEthernet
	enum PIP_EXPORT Type {
		UDP /** UDP - User Datagram Protocol */ ,
		TCP_Client /** TCP client - allow connection to TCP server */ ,
		TCP_Server /** TCP server - receive connections from TCP clients */ ,
		TCP_SingleTCP /** TCP client single mode - connect & send & disconnect, on each packet */
	};
	
	//! \brief Parameters of %PIEthernet
	enum PIP_EXPORT Parameters {
		ReuseAddress /** Rebind address if there is already binded. Enabled by default */ = 0x1,
		Broadcast /** Broadcast send. Disabled by default */ = 0x2,
		SeparateSockets /** If this parameter is set, %PIEthernet will initialize two different sockets,
							for receive and send, instead of single one. Disabled by default */ = 0x4,
		MulticastLoop /** Enable receiving multicast packets from same host. Enabled by default */ = 0x8,
		KeepConnection /** Automatic reconnect TCP connection on disconnect. Enabled by default */ = 0x10,
		DisonnectOnTimeout /** Disconnect TCP connection on read timeout expired. Disabled by default */ = 0x20
	};

	//! Contructs %PIEthernet with type "type", read address "ip_port" and parameters "params"
	explicit PIEthernet(Type type, const PIString & ip_port = PIString(), const PIFlags<Parameters> params = PIEthernet::ReuseAddress | PIEthernet::MulticastLoop | PIEthernet::KeepConnection);
	
	virtual ~PIEthernet();

	
	//! Set read address
	void setReadAddress(const PIString & ip, int port) {setPath(ip + PIStringAscii(":") + PIString::fromNumber(port));}
	
	//! Set read address in format "i.i.i.i:p"
	void setReadAddress(const PIString & ip_port) {setPath(ip_port);}
	
	//! Set read IP
	void setReadIP(const PIString & ip) {parseAddress(path(), &ip_, &port_); setPath(ip + PIStringAscii(":") + PIString::fromNumber(port_));}
	
	//! Set read port
	void setReadPort(int port) {parseAddress(path(), &ip_, &port_); setPath(ip_ + PIStringAscii(":") + PIString::fromNumber(port));}
	
	
	//! Set send address
	void setSendAddress(const PIString & ip, int port) {ip_s = ip; port_s = port;}
	
	//! Set send address in format "i.i.i.i:p"
	void setSendAddress(const PIString & ip_port) {parseAddress(ip_port, &ip_s, &port_s);}
	
	//! Set send IP
	void setSendIP(const PIString & ip) {ip_s = ip;}
	
	//! Set send port
	void setSendPort(int port) {port_s = port;}
	
	
	//! Returns read address in format "i.i.i.i:p"
	PIString readAddress() const {return path();}
	
	//! Returns read IP
	PIString readIP() const {parseAddress(path(), &ip_, &port_); return ip_;}
	
	//! Returns read port
	int readPort() const {parseAddress(path(), &ip_, &port_); return port_;}
	
	
	//! Returns send address in format "i.i.i.i:p"
	PIString sendAddress() const {return ip_s + PIStringAscii(":") + PIString::fromNumber(port_s);}
	
	//! Returns send IP
	PIString sendIP() const {return ip_s;}
	
	//! Returns send port
	int sendPort() const {return port_s;}
	
	
	//! Returns address of last received UDP packet in format "i.i.i.i:p"
	PIString lastReadAddress() const {return ip_r + PIStringAscii(":") + PIString::fromNumber(port_r);}
	
	//! Returns IP of last received UDP packet
	PIString lastReadIP() const {return ip_r;}
	
	//! Returns port of last received UDP packet
	int lastReadPort() const {return port_r;}

	
	//! Set parameters to "parameters_". You should to reopen %PIEthernet to apply them
	void setParameters(PIFlags<PIEthernet::Parameters> parameters_) {setProperty(PIStringAscii("parameters"), (int)parameters_);}
	
	//! Set parameter "parameter" to state "on". You should to reopen %PIEthernet to apply this
	void setParameter(PIEthernet::Parameters parameter, bool on = true);
	
	//! Returns if parameter "parameter" is set
	bool isParameterSet(PIEthernet::Parameters parameter) const {return ((PIFlags<PIEthernet::Parameters>)(property(PIStringAscii("parameters")).toInt()))[parameter];}
	
	//! Returns parameters
	PIFlags<PIEthernet::Parameters> parameters() const {return (PIFlags<PIEthernet::Parameters>)(property(PIStringAscii("parameters")).toInt());}

	//PIByteArray macAddress() {if (!init_) init(); struct ifreq ifr; memset(&ifr, 0, sizeof(ifr)); memcpy(ifr.ifr_name, "eth0", 5); ioctl(sock, SIOCSIFHWADDR, &ifr); return PIByteArray(&ifr.ifr_hwaddr.sa_data, 6);}
	
	//! Returns %PIEthernet type
	Type type() const {return (Type)(property(PIStringAscii("type")).toInt());}
	
	//! Returns read timeout
	double readTimeout() const {return property(PIStringAscii("readTimeout")).toDouble();}
	
	//! Returns write timeout
	double writeTimeout() const {return property(PIStringAscii("writeTimeout")).toDouble();}
	
	//! Set timeout for read
	void setReadTimeout(double ms) {setProperty(PIStringAscii("readTimeout"), ms);}
	
	//! Set timeout for write
	void setWriteTimeout(double ms) {setProperty(PIStringAscii("writeTimeout"), ms);}
	
	
	//! Returns TTL (Time To Live)
	int TTL() const {return property(PIStringAscii("TTL")).toInt();}
	
	//! Returns multicast TTL (Time To Live)
	int multicastTTL() const {return property(PIStringAscii("MulticastTTL")).toInt();}
	
	//! Set TTL (Time To Live), default is 64
	void setTTL(int ttl) {setProperty(PIStringAscii("TTL"), ttl);}
	
	//! Set multicast TTL (Time To Live), default is 1
	void setMulticastTTL(int ttl) {setProperty(PIStringAscii("MulticastTTL"), ttl);}

	
	//! Join to multicast group with address "group". Use only for UDP
	bool joinMulticastGroup(const PIString & group);
	
	//! Leave multicast group with address "group". Use only for UDP
	bool leaveMulticastGroup(const PIString & group);
	
	//! Returns joined multicast groups. Use only for UDP
	const PIStringList & multicastGroups() const {return mcast_groups;}
	
	
	//! Connect to TCP server with address \a readAddress(). Use only for TCP_Client
	bool connect();
	
	//! Connect to TCP server with address "ip":"port". Use only for TCP_Client
	bool connect(const PIString & ip, int port) {setPath(ip + PIStringAscii(":") + PIString::fromNumber(port)); return connect();}
	
	//! Connect to TCP server with address "ip_port". Use only for TCP_Client
	bool connect(const PIString & ip_port) {setPath(ip_port); return connect();}
	
	//! Returns if %PIEthernet connected to TCP server. Use only for TCP_Client
	bool isConnected() const {return connected_;}
	
	//! Returns if %PIEthernet is connecting to TCP server. Use only for TCP_Client
	bool isConnecting() const {return connecting_;}
	
	
	//! Start listen for incoming TCP connections on address \a readAddress(). Use only for TCP_Server
	bool listen(bool threaded = false);
	
	//! Start listen for incoming TCP connections on address "ip":"port". Use only for TCP_Server
	bool listen(const PIString & ip, int port, bool threaded = false) {setReadAddress(ip, port); return listen(threaded);}
	
	//! Start listen for incoming TCP connections on address "ip_port". Use only for TCP_Server
	bool listen(const PIString & ip_port, bool threaded = false) {setReadAddress(ip_port); return listen(threaded);}

	PIEthernet * client(int index) {return clients_[index];}
	int clientsCount() const {return clients_.size_s();}
	PIVector<PIEthernet * > clients() const {return clients_;}

	
	//! Send data "data" with size "size" to address \a sendAddress() for UDP or \a readAddress() for TCP_Client
	bool send(const void * data, int size, bool threaded = false) {if (threaded) {writeThreaded(data, size); return true;} return (write(data, size) == size);}
	
	//! Send data "data" with size "size" to address "ip":"port"
	bool send(const PIString & ip, int port, const void * data, int size, bool threaded = false) {ip_s = ip; port_s = port; if (threaded) {writeThreaded(data, size); return true;} return send(data, size);}
	
	//! Send data "data" with size "size" to address "ip_port"
	bool send(const PIString & ip_port, const void * data, int size, bool threaded = false) {parseAddress(ip_port, &ip_s, &port_s); if (threaded) {writeThreaded(data, size); return true;} return send(data, size);}
	
	//! Send data "data" to address \a sendAddress() for UDP or \a readAddress() for TCP_Client
	bool send(const PIByteArray & data, bool threaded = false) {if (threaded) {writeThreaded(data); return true;} return (write(data) == data.size_s());}
	
	//! Send data "data" to address "ip":"port" for UDP
	bool send(const PIString & ip, int port, const PIByteArray & data, bool threaded = false) {ip_s = ip; port_s = port; if (threaded) {writeThreaded(data); return true;} return send(data);}
	
	//! Send data "data" to address "ip_port" for UDP
	bool send(const PIString & ip_port, const PIByteArray & data, bool threaded = false) {parseAddress(ip_port, &ip_s, &port_s); if (threaded) {writeThreaded(data); return true;} return (write(data) == data.size_s());}
	
	virtual bool canWrite() const {return mode() & WriteOnly;}
	
	EVENT1(newConnection, PIEthernet * , client)
	EVENT0(connected)
	EVENT1(disconnected, bool, withError)
	
	
	//! Flags of network interface
	enum PIP_EXPORT InterfaceFlag {
		ifActive /** Is active */ = 0x1,
		ifRunning /** Is running */ = 0x2,
		ifBroadcast /** Support broadcast */ = 0x4,
		ifMulticast /** Support multicast */ = 0x8,
		ifLoopback /** Is loopback */ = 0x10,
		ifPTP /** Is point-to-point */ = 0x20
	};
	
	//! %PIFlags of network interface flags
	typedef PIFlags<InterfaceFlag> InterfaceFlags;
	
	
	//! Network interface descriptor
	struct PIP_EXPORT Interface {
		
		//! System index
		int index;
		
		//! MTU
		int mtu;
		
		//! System name
		PIString name;
		
		//! MAC address in format "hh:hh:hh:hh:hh:hh" or empty if there is no MAC address
		PIString mac;
		
		//! IP address in format "i.i.i.i" or empty if there is no IP address
		PIString address;
		
		//! Netmask of IP address in format "i.i.i.i" or empty if there is no netmask
		PIString netmask;
		
		//! Broadcast address in format "i.i.i.i" or empty if there is no broadcast address
		PIString broadcast;
		
		//! Point-to-point address or empty if there is no point-to-point address
		PIString ptp;
		
		//! Flags of interface
		InterfaceFlags flags;
		
		//! Returns if interface is active
		bool isActive() const {return flags[PIEthernet::ifActive];}
		
		//! Returns if interface is running
		bool isRunning() const {return flags[PIEthernet::ifRunning];}
		
		//! Returns if interface support broadcast
		bool isBroadcast() const {return flags[PIEthernet::ifBroadcast];}
		
		//! Returns if interface support multicast
		bool isMulticast() const {return flags[PIEthernet::ifMulticast];}
		
		//! Returns if interface is loopback
		bool isLoopback() const {return flags[PIEthernet::ifLoopback];}
		
		//! Returns if interface is point-to-point
		bool isPTP() const {return flags[PIEthernet::ifPTP];}
	};
	
	
	//! Array of \a Interface with some features
	class PIP_EXPORT InterfaceList: public PIVector<PIEthernet::Interface> {
	public:
		InterfaceList(): PIVector<PIEthernet::Interface>() {}
		
		//! Get interface with system index "index" or 0 if there is no one
		const Interface * getByIndex(int index) const {for (int i = 0; i < size_s(); ++i) if ((*this)[i].index == index) return &((*this)[i]); return 0;}
		
		//! Get interface with system name "name" or 0 if there is no one
		const Interface * getByName(const PIString & name) const {for (int i = 0; i < size_s(); ++i) if ((*this)[i].name == name) return &((*this)[i]); return 0;}
		
		//! Get interface with IP address "address" or 0 if there is no one
		const Interface * getByAddress(const PIString & address) const {for (int i = 0; i < size_s(); ++i) if ((*this)[i].address == address) return &((*this)[i]); return 0;}
		
		//! Get loopback interface or 0 if there is no one
		const Interface * getLoopback() const {for (int i = 0; i < size_s(); ++i) if ((*this)[i].isLoopback()) return &((*this)[i]); return 0;}
	};

	
	//! Returns all system network interfaces
	static InterfaceList interfaces();
	
	static PIString interfaceAddress(const PIString & interface_);
	
	//! Returns all system network IP addresses
	static PIStringList allAddresses();
	
	//! Resolve hostname "host" and return it IPv4 address or "0.0.0.0" on error
	static PIString resolve(const PIString & host);

	static void parseAddress(const PIString & ipp, PIString * ip, int * port);
	static PIString macFromBytes(const PIByteArray & mac);
	static PIByteArray macToBytes(const PIString & mac);
	static PIString applyMask(const PIString & ip, const PIString & mask);
	static PIString getBroadcast(const PIString & ip, const PIString & mask);

//! \events
//! \{

	//! \fn void newConnection(PIEthernet * client)
	//! \brief Raise on new TCP connection received
	
	//! \fn void connected()
	//! \brief Raise if succesfull TCP connection

	//! \fn void disconnected(bool withError)
	//! \brief Raise if TCP connection was closed

//! \}
//! \ioparams
//! \{
#ifdef DOXYGEN
	//! \brief read ip, default ""
	string ip;
	
	//! \brief read port, default 0
	int port;
	
	//! \brief ethernet parameters
	int parameters;
	
	//! \brief read timeout, default 1000 ms
	double readTimeout;
	
	//! \brief write timeout, default 1000 ms
	double writeTimeout;
	
	//! \brief time-to-live, default 64
	int TTL;
	
	//! \brief time-to-live for multicast, default 1
	int multicastTTL;
#endif
//! \}

protected:
	explicit PIEthernet(int sock, PIString ip_port);

	void propertyChanged(const PIString & name);
	
	PIString fullPathPrefix() const {return PIStringAscii("eth");}
	PIString constructFullPathDevice() const;
	void configureFromFullPathDevice(const PIString & full_path);
	bool configureDevice(const void * e_main, const void * e_parent = 0);
	int readDevice(void * read_to, int max_size);
	int writeDevice(const void * data, int max_size);

	//! Executes when any read function was successful. Default implementation does nothing
	virtual void received(const void * data, int size) {;}

	void construct();
	bool init();
	bool openDevice();
	bool closeDevice();
	void closeSocket(int & sd);
	void applyTimeouts();
	void applyTimeout(int fd, int opt, double ms);
	void applyOptInt(int level, int opt, int val);
	
	PRIVATE_DECLARATION
	int sock, sock_s;
	mutable int port_, port_s, port_r;
	bool connected_, connecting_, listen_threaded, server_bounded;
	mutable PIString ip_, ip_s, ip_r;
	PIThread server_thread_;
	PIMutex clients_mutex;
	PIVector<PIEthernet * > clients_;
	PIQueue<PIString> mcast_queue;
	PIStringList mcast_groups;

private:
	EVENT_HANDLER(void, clientDeleted);
	static void server_func(void * eth);
	void setType(Type t, bool reopen = true) {setProperty(PIStringAscii("type"), (int)t); if (reopen && isOpened()) {closeDevice(); init(); openDevice();}}

	inline static int ethErrorCore();
	inline static PIString ethErrorString();
	inline static int ethRecv(int sock, void * buf, int size, int flags = 0);
	inline static int ethRecvfrom(int sock, void * buf, int size, int flags, sockaddr * addr);
	inline static int ethSendto(int sock, const void * buf, int size, int flags, sockaddr * addr, int addr_len);
	inline static void ethClosesocket(int sock, bool shutdown);
	inline static int ethSetsockopt(int sock, int level, int optname, const void * optval, int optlen);
	inline static int ethSetsockoptInt(int sock, int level, int optname, int value = 1);
	inline static int ethSetsockoptBool(int sock, int level, int optname, bool value = true);

};

inline bool operator <(const PIEthernet::Interface & v0, const PIEthernet::Interface & v1) {return (v0.name < v1.name);}
inline bool operator ==(const PIEthernet::Interface & v0, const PIEthernet::Interface & v1) {return (v0.name == v1.name && v0.address == v1.address && v0.netmask == v1.netmask);}
inline bool operator !=(const PIEthernet::Interface & v0, const PIEthernet::Interface & v1) {return (v0.name != v1.name || v0.address != v1.address || v0.netmask != v1.netmask);}

#endif // PIETHERNET_H
