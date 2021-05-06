/*! \file piusb.h
 * \brief USB device
*/
/*
	PIP - Platform Independent Primitives
	USB, based on libusb
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

#ifndef PIUSB_H
#define PIUSB_H

#include "piiodevice.h"

struct usb_dev_handle;

class PIP_EXPORT PIUSB: public PIIODevice
{
	PIIODEVICE(PIUSB)
public:
	PIUSB(ushort vid = 0, ushort pid = 0);
	~PIUSB() {closeDevice();}

	struct Endpoint {
		Endpoint(uchar a = 0, uchar at = 0, ushort mps = 0) {address = a; attributes = at; max_packet_size = mps; parse();}

		enum Direction {Write = 0, Read = 1};
		enum TransferType {Control = 0, Isochronous = 1, Bulk = 2, Interrupt = 3};
		enum SynchronisationType {NoSynchonisation= 0, Asynchronous = 2, Adaptive = 1, Synchronous = 3};
		enum UsageType {DataEndpoint = 0, FeedbackEndpoint = 2, ExplicitFeedbackDataEndpoint = 1};

		void parse();
		bool isNull() const {return address == 0;}

		uchar address;
		uchar attributes;
		ushort max_packet_size;
		Direction direction;
		TransferType transfer_type;
		SynchronisationType synchronisation_type;
		UsageType usage_type;
	};

	struct Interface {
		Interface() {index = value_to_select = class_code = subclass_code = protocol_code = 0;}
		uchar index;
		uchar value_to_select;
		ushort class_code;
		ushort subclass_code;
		ushort protocol_code;
		PIVector<PIUSB::Endpoint> endpoints;
	};

	struct Configuration {
		Configuration() {index = value_to_select = attributes = max_power = 0; self_powered = remote_wakeup = false;}
		uchar index;
		uchar value_to_select;
		uchar attributes;
		ushort max_power; // mA
		bool self_powered;
		bool remote_wakeup;
		PIVector<PIUSB::Interface> interfaces;
	};

	struct Descriptor {
		Descriptor() {memset(this, 0, sizeof(Descriptor));}
		ushort usb_spec_number;
		uchar device_class;
		uchar device_subclass;
		uchar device_protocol;
		uchar max_packet_size;
		ushort id_vendor;
		ushort id_product;
		ushort id_device_release;
		uchar index_manufacturer;
		uchar index_product;
		uchar index_serial;
		PIVector<PIUSB::Configuration> configurations;
	};

	const PIUSB::Descriptor & currentDescriptor() const {return desc_;}
	const PIUSB::Configuration & currentConfiguration() const {return conf_;}
	const PIUSB::Interface & currentInterface() const {return iface_;}

	ushort vendorID() const {return vid_;}
	ushort productID() const {return pid_;}

	int deviceNumber() const {return property("deviceNumber").toInt();}
	int timeoutRead() const {return property("timeoutRead").toInt();}
	int timeoutWrite() const {return property("timeoutWrite").toInt();}
	const PIUSB::Endpoint & endpointRead() const {return ep_read;}
	const PIUSB::Endpoint & endpointWrite() const {return ep_write;}

	const PIVector<PIUSB::Endpoint> & endpoints() const {return eps;}
	PIVector<PIUSB::Endpoint> endpointsRead();
	PIVector<PIUSB::Endpoint> endpointsWrite();
	PIUSB::Endpoint getEndpointByAddress(uchar address);

	void setVendorID(ushort vid) {vid_ = vid; setPath(PIString::fromNumber(vid_, 16).expandLeftTo(4, "0") + ":" + PIString::fromNumber(pid_, 16).expandLeftTo(4, "0"));}
	void setProductID(ushort pid) {pid_ = pid; setPath(PIString::fromNumber(vid_, 16).expandLeftTo(4, "0") + ":" + PIString::fromNumber(pid_, 16).expandLeftTo(4, "0"));}

	bool setConfiguration(uchar value);
	bool setInterface(uchar value);

	void setEndpointRead(const PIUSB::Endpoint & ep) {ep_read = ep;}
	void setEndpointWrite(const PIUSB::Endpoint & ep) {ep_write = ep;}
	void setDeviceNumber(int dn) {setProperty("deviceNumber", dn);}
	void setTimeoutRead(int t) {setProperty("timeoutRead", t);}
	void setTimeoutWrite(int t) {setProperty("timeoutWrite", t);}

	int controlWrite(const void * data, int max_size);

	void flush();

protected:
	PIString fullPathPrefix() const {return "usb";}
	bool configureDevice(const void * e_main, const void * e_parent = 0);
	PIString constructFullPathDevice() const;
	void configureFromFullPathDevice(const PIString & full_path);
	int readDevice(void * read_to, int max_size);
	int writeDevice(const void * data, int max_size);
	//bool init();
	bool openDevice();
	bool closeDevice();

	PIVector<PIUSB::Endpoint> eps;
	ushort vid_, pid_;
	int intefrace_, timeout_r, timeout_w;
	int interface_claimed;
	PIUSB::Endpoint ep_read, ep_write;
	Descriptor desc_;
	Configuration conf_;
	Interface iface_;
	usb_dev_handle * hdev;

};

PICout operator <<(PICout s, const PIUSB::Endpoint & v);

#endif // PIUSB_H
