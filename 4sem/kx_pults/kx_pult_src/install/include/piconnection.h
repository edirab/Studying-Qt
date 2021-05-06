/*! \file piconnection.h
 * \brief Complex I/O point
*/
/*
    PIP - Platform Independent Primitives
    Complex I/O point
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

#ifndef PICONNECTION_H
#define PICONNECTION_H

#include "pipacketextractor.h"
#include "pidiagnostics.h"

class PIConfig;

class PIP_EXPORT PIConnection: public PIObject
{
	PIOBJECT_SUBCLASS(PIConnection, PIObject)
public:
	//! Constructs connection with name "name", or with default name = "connection"
	PIConnection(const PIString & name = PIStringAscii("connection"));
	
	//! Constructs connection and configure it from config file "config" from section "name"
	PIConnection(const PIString & config, const PIString & name);
	
	//! Constructs connection and configure it from config content "string" from section "name"
	PIConnection(PIString * string, const PIString & name);

	~PIConnection();
	
	
	/*! \brief Configure connection from config file "config" from section "name". Returns if configuration was successful
	 * \details \b Warning: all devices, filters and channels removed before configure! */
	bool configureFromConfig(const PIString & config, const PIString & name = PIStringAscii("connection"));
	
	/*! \brief Configure connection from config content "string" from section "name". Returns if configuration was successful
	 * \details \b Warning: all devices, filters and channels removed before configure! */
	bool configureFromString(PIString * string, const PIString & name = PIStringAscii("connection"));
	
	//! Returns config file section of current connection configuration
	PIString makeConfig() const;
	
	
	/*! \brief Add device with full path "full_path", open mode "mode" to Device pool and connection
	 * \details Returns pointer to device or null if device can not be created. If "start" is true,
	 * read thread is started immediately. Else, you can start read thread with functions \a startThreadedRead()
	 * or \a startAllThreadedReads(). By default, read thread doesn`t start */
	PIIODevice * addDevice(const PIString & full_path, PIIODevice::DeviceMode mode = PIIODevice::ReadWrite, bool start = false);
	
	void setDeviceName(PIIODevice * dev, const PIString & name) {device_names[name] = dev;}
	
	PIStringList deviceNames(const PIIODevice * dev) const;
	
	/*! \brief Remove device with full path "full_path" from connection
	 * \details Returns if device was removed. If there is no connection bounded to this device,
	 * it will be removed from Device pool */
	bool removeDevice(const PIString & full_path);
	
	/*! \brief Remove all device from connection
	 * \details If there is no connection bounded to there devices, they removed from Device pool */
	void removeAllDevices();
	
	//! Returns device with full path "full_path" or null if there is no such device
	PIIODevice * deviceByFullPath(const PIString & full_path) const;
	
	//! Returns device with name "name" or null if there is no such device
	PIIODevice * deviceByName(const PIString & name) const;
	
	//! Returns all devices bounded to this connection
	PIVector<PIIODevice * > boundedDevices() const;
	
	
	/*! \brief Add filter with name "name" to device with full path "full_path_name" or filter "full_path_name"
	 * \details If there is no filter with name "name", connection create new with split mode "mode" and bound
	 * to it device "full_path_name" or filter "full_path_name". If filter with name "name" already exists,
	 * device "full_path_name" or filter "full_path_name" add to this filter.
	 * This function returns PIPacketExtractor * assosiated with this filter
	 * \n \b Attention! "mode" is altual olny if new filter was created! */
	PIPacketExtractor * addFilter(const PIString & name, const PIString & full_path_name, PIPacketExtractor::SplitMode mode = PIPacketExtractor::None);
	
	//! Add filter with name "name" to device "dev"
	PIPacketExtractor * addFilter(const PIString & name, const PIIODevice * dev, PIPacketExtractor::SplitMode mode = PIPacketExtractor::None) {return addFilter(name, devFPath(dev), mode);}

	//! Add filter with "filter" to device "dev"
	PIPacketExtractor * addFilter(PIPacketExtractor * filter, const PIString & full_path_name);

	//! Add filter with "filter" to device "dev"
	PIPacketExtractor * addFilter(PIPacketExtractor * filter, const PIIODevice * dev) {return addFilter(filter, devFPath(dev));}
	
	/*! \brief Remove from filter with name "name" device with full path "full_path_name" or filter "full_path_name"
	 * \details If there is no devices bounded to this filter, it will be removed. Returns if device was removed */
	bool removeFilter(const PIString & name, const PIString & full_path_name);
	
	//! Remove from filter with name "name" device or filter "dev"
	bool removeFilter(const PIString & name, const PIIODevice * dev);
	
	//! Remove filter with name "name". Returns if filter was removed
	bool removeFilter(const PIString & name);
	
	//! Remove all filters from connection
	void removeAllFilters();
	
	
	//! Returns all filters of connection
	PIVector<PIPacketExtractor * > filters() const;
	
	//! Returns all filter names of connection
	PIStringList filterNames() const;
	
	//! Returns PIPacketExtractor * assosiated with filter "name" or null if there is no such filter
	PIPacketExtractor * filter(const PIString & name) const;
	
	//! Returns all devices bounded to filter "name"
	PIVector<PIIODevice * > filterBoundedDevices(const PIString & name) const;
	
	
	/*! \brief Add to connection channel from "name_from" to "name_to"
	 * \details "name_from" and "name_to" can be full pathes of devices or filter names.
	 * Returns \b false if there if no such device or filter, else create channel and returns \b true */
	bool addChannel(const PIString & name_from, const PIString & name_to);
	
	//! Add to connection channel from "name_from" to "dev_to"
	bool addChannel(const PIString & name_from, const PIIODevice * dev_to) {return addChannel(name_from, devFPath(dev_to));}
	
	//! Add to connection channel from "dev_from" to "name_to"
	bool addChannel(const PIIODevice * dev_from, const PIString & name_to) {return addChannel(devFPath(dev_from), name_to);}
	
	//! Add to connection channel from "dev_from" to "dev_to"
	bool addChannel(const PIIODevice * dev_from, const PIIODevice * dev_to) {return addChannel(devFPath(dev_from), devFPath(dev_to));}
	
	/*! \brief Remove from connection channel from "name_from" to "name_to"
	 * \details "name_from" and "name_to" can be full pathes of devices or filter names.
	 * Returns \b false if there if no such device or filter, else remove channel and returns \b true */
	bool removeChannel(const PIString & name_from, const PIString & name_to);
	
	//! Remove from connection channel from "name_from" to "dev_to"
	bool removeChannel(const PIString & name_from, const PIIODevice * dev_to) {return removeChannel(name_from, devFPath(dev_to));}
	
	//! Remove from connection channel from "dev_from" to "name_to"
	bool removeChannel(const PIIODevice * dev_from, const PIString & name_to) {return removeChannel(devFPath(dev_from), name_to);}
	
	//! Remove from connection channel from "dev_from" to "dev_to"
	bool removeChannel(const PIIODevice * dev_from, const PIIODevice * dev_to) {return removeChannel(devFPath(dev_from), devFPath(dev_to));}
	
	/*! \brief Remove from connection all channels from "name_from"
	 * \details "name_from" can be full path of device or filter name.
	 * Returns \b false if there if no such device or filter, else remove channels and returns \b true */
	bool removeChannel(const PIString & name_from);
	
	//! Remove from connection all channels from "dev_from"
	bool removeChannel(const PIIODevice * dev_from) {return removeChannel(devFPath(dev_from));}
	
	//! Remove from connection all channels
	void removeAllChannels();
	
	//! Returns all channels of this connection as full pathes or filter names pair array (from, to)
	PIVector<PIPair<PIString, PIString> > channels() const;
	
	
	/*! \brief Add to connection sender with name "name" device with full path "full_path"
	 * \details If there is no sender with name "name", connection create new, bound
	 * to it device "full_path_name" and start sender timer with frequency "frequency".
	 * If sender with name "name" already exists, device "full_path_name" add to this sender
	 * If "start" is true, sender is started immediately. Else, you can start sender with
	 * functions \a startSender()
	 * \n \b Attention! "frequency" is actual olny if new sender was created! */
	void addSender(const PIString & name, const PIString & full_path, float frequency, bool start = false);
	
	//! Add to connection sender with name "name" device "dev"
	void addSender(const PIString & name, const PIIODevice * dev, float frequency, bool start = false) {addSender(name, devFPath(dev), frequency, start);}
	
	/*! \brief Remove from sender with name "name" device with full path "full_path_name"
	 * \details If there is no devices bounded to this sender, it will be removed. Returns if sender was removed */
	bool removeSender(const PIString & name, const PIString & full_path);
	
	//! Remove from sender with name "name" device "dev"
	bool removeSender(const PIString & name, const PIIODevice * dev) {return removeSender(name, devFPath(dev));}
	
	//! Remove sender with name "name", returns if sender was removed
	bool removeSender(const PIString & name);
	
	//! Set sender "name" fixed send data "data", returns if sender exists
	bool setSenderFixedData(const PIString & name, const PIByteArray & data);
	
	//! Remove sender "name" fixed send data, returns if sender exists
	bool clearSenderFixedData(const PIString & name);
	
	//! Returns sender "name" fixed send data
	PIByteArray senderFixedData(const PIString & name) const;
	
	//! Returns sender "name" timer frequency, -1 if there is no such sender, or 0 if sender is not started yet
	float senderFrequency(const PIString & name) const;
	
	//! Remove from connection all senders
	void removeAllSenders();
	
	
	//! Start read thread of device with full path "full_path"
	void startThreadedRead(const PIString & full_path);
	
	//! Start read thread of device "dev"
	void startThreadedRead(const PIIODevice * dev) {startThreadedRead(devFPath(dev));}
	
	//! Start read threads of all Device pool device
	void startAllThreadedReads();
	
	//! Start sender "name" timer
	void startSender(const PIString & name);
	
	//! Start all senders timers
	void startAllSenders();
	
	//! Start all read threads and senders
	void start() {startAllThreadedReads(); startAllSenders();}
	
	//! Stop read thread of device with full path "full_path"
	void stopThreadedRead(const PIString & full_path);
	
	//! Stop read thread of device "dev"
	void stopThreadedRead(const PIIODevice * dev) {stopThreadedRead(devFPath(dev));}
	
	//! Stop read threads of all Device pool device
	void stopAllThreadedReads();
	
	//! Stop sender "name" timer
	void stopSender(const PIString & name);
	
	//! Stop all senders timers
	void stopAllSenders();
	
	//! Stop all read threads and senders
	void stop() {stopAllThreadedReads(); stopAllSenders();}
	
	//! Stop connection and remove all devices
	void destroy() {stop(); removeAllDevices();}
	
	//! Returns if there are no devices in this connection
	bool isEmpty() const {return device_modes.isEmpty();}
	
	
	//! Returns PIDiagnostics * assosiated with device with full path "full_path_name", name "full_path_name" or filter "full_path_name"
	PIDiagnostics * diagnostic(const PIString & full_path_name) const;
	
	//! Returns PIDiagnostics * assosiated with device or filter "dev"
	PIDiagnostics * diagnostic(const PIIODevice * dev) const {return diags_.value(const_cast<PIIODevice * >(dev), 0);}
	
	
	//! Write data "data" to device with full path "full_path" and returns result of \a write() function of device
	int writeByFullPath(const PIString & full_path, const PIByteArray & data);
	
	//! Write data "data" to device with name "name" and returns result of \a write() function of device
	int writeByName(const PIString & name, const PIByteArray & data);
	
	//! Write data "data" to device "dev" and returns result of \a write() function of device
	int write(PIIODevice * dev, const PIByteArray & data);
	
	
	//! Returns all connections in application
	static PIVector<PIConnection * > allConnections();
	
	//! Returns all devices in Device pool
	static PIVector<PIIODevice * > allDevices();
	
	//! Set Device pool fake mode to \"yes\" and returns previous mode
	static bool setFakeMode(bool yes);
	
	//! Returns if Device pool works in fake mode
	static bool isFakeMode();
	
	class DevicePool: public PIThread {
		PIOBJECT_SUBCLASS(DevicePool, PIThread)
		friend void __DevicePool_threadReadDP(void * ddp);
		friend class PIConnection;
	public:
		DevicePool();
		
		void init();
		PIIODevice * addDevice(PIConnection * parent, const PIString & fp, PIIODevice::DeviceMode mode = PIIODevice::ReadWrite, bool start = true);
		bool removeDevice(PIConnection * parent, const PIString & fp);
		void unboundConnection(PIConnection * parent);
		PIIODevice * device(const PIString & fp) const;
		PIVector<PIConnection * > boundedConnections() const;
		PIVector<PIIODevice * > boundedDevices() const;
		PIVector<PIIODevice * > boundedDevices(const PIConnection * parent) const;
		
	protected:
		struct DeviceData {
			DeviceData(): dev(0), rthread(0), started(false) {}
			~DeviceData();
			PIIODevice * dev;
			PIThread * rthread;
			bool started;
			PIVector<PIConnection * > listeners;
		};
		
		void run();
		
		void deviceReaded(DeviceData * dd, const PIByteArray & data);
		
		typedef PIMap<PIString, DeviceData * >::value_type DDPair;
		PIMap<PIString, DeviceData * > devices;
		bool fake;
	};
	
	EVENT2(dataReceivedEvent, const PIString &, from, const PIByteArray &, data)
	EVENT2(packetReceivedEvent, const PIString &, from, const PIByteArray &, data)
	EVENT3(qualityChanged, const PIIODevice * , dev, PIDiagnostics::Quality, new_quality, PIDiagnostics::Quality, old_quality)
	
//! \events
//! \{
	
	//! \fn void dataReceivedEvent(const PIString & from, const PIByteArray & data)
	//! \brief Raise on data received from device with full path "from"
	
	//! \fn void packetReceivedEvent(const PIString & from, const PIByteArray & data)
	//! \brief Raise on packet received from filter with name "from"
	
	//! \fn void qualityChanged(const PIIODevice * device, PIDiagnostics::Quality new_quality, PIDiagnostics::Quality old_quality)
	//! \brief Raise on diagnostic quality of device "device" changed from "old_quality" to "new_quality"
	
//! \}
	
protected:
	
	//! Executes on data received from device with full path "from"
	virtual void dataReceived(const PIString & from, const PIByteArray & data) {}
	
	//! Executes on packet received from filter with name "from"
	virtual void packetReceived(const PIString & from, const PIByteArray & data) {}
	
	//! Validate header "rec" with source header "src" and size "size", executes from filter "filter_name"
	virtual bool filterValidateHeader(const PIString & filter_name, uchar * src, uchar * rec, int size);
	
	//! Validate footer "rec" with source footer "src" and size "size", executes from filter "filter_name"
	virtual bool filterValidateFooter(const PIString & filter_name, uchar * src, uchar * rec, int size);
	
	//! Validate payload "rec" with size "size", executes from filter "filter_name"
	virtual bool filterValidatePayload(const PIString & filter_name, uchar * rec, int size);
	
	//! You should returns data for sender "sender_name"
	virtual PIByteArray senderData(const PIString & sender_name);
	
private:
	static bool filterValidateHeaderS(void * c, uchar * src, uchar * rec, int size);
	static bool filterValidateFooterS(void * c, uchar * src, uchar * rec, int size);
	static bool filterValidatePayloadS(void * c, uchar * rec, int size);
	bool configure(PIConfig & conf, const PIString & name_);
	void rawReceived(PIIODevice * dev, const PIString & from, const PIByteArray & data);
	void unboundExtractor(PIPacketExtractor * pe);
	EVENT_HANDLER2(void, packetExtractorReceived, uchar * , data, int, size);
	EVENT_HANDLER2(void, diagQualityChanged, PIDiagnostics::Quality, new_quality, PIDiagnostics::Quality, old_quality);
	
	PIString devPath(const PIIODevice * d) const;
	PIString devFPath(const PIIODevice * d) const;
	
	struct Extractor {
		Extractor(): extractor(0) {}
		~Extractor();
		PIPacketExtractor * extractor;
		PIVector<PIIODevice * > devices;
	};
	
	class Sender: public PITimer {
		PIOBJECT_SUBCLASS(Sender, PIObject)
	public:
		Sender(PIConnection * parent_ = 0): parent(parent_), int_(0.f) {needLockRun(true);}
		~Sender() {stop();}
		PIConnection * parent;
		PIVector<PIIODevice * > devices;
		PIByteArray sdata;
		float int_;
		void tick(void * , int);
	};
	
	typedef PIMap<PIString, Extractor * >::value_type PEPair;
	typedef PIMap<PIString, Sender * >::value_type SPair;
	typedef PIMap<PIString, PIIODevice * >::value_type DNPair;
	typedef PIMap<PIIODevice * , PIVector<PIPacketExtractor * > >::value_type BEPair;
	typedef PIMap<PIIODevice * , PIVector<PIIODevice * > >::value_type CPair;
	typedef PIMap<PIIODevice * , PIDiagnostics * >::value_type DPair;
	PIMap<PIString, Extractor * > extractors;
	PIMap<PIString, Sender * > senders;
	PIMap<PIString, PIIODevice * > device_names;
	PIMap<PIIODevice * , PIIODevice::DeviceMode> device_modes;
	PIMap<PIIODevice * , PIVector<PIPacketExtractor * > > bounded_extractors;
	PIMap<PIIODevice * , PIVector<PIIODevice * > > channels_;
	PIMap<PIIODevice * , PIDiagnostics * > diags_;
	
	static PIVector<PIConnection * > _connections;
	
};

void __DevicePool_threadReadDP(void * ddp);


extern PIConnection::DevicePool * __device_pool__;

class __DevicePoolContainer__ {
public:
	__DevicePoolContainer__();
	static bool inited_;
};

static __DevicePoolContainer__ __device_pool_container__;


#endif // PICONNECTION_H
