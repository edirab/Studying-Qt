/*! \file pipacketextractor.h
 * \brief Packets extractor
*/
/*
	PIP - Platform Independent Primitives
	Packets extractor
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


#ifndef PIPACKETEXTRACTOR_H
#define PIPACKETEXTRACTOR_H

#include "piiodevice.h"

// Pass data, recHeaderPtr, received_data, recHeaderSize. Return true if packet is correct nor return false.
typedef bool (*PacketExtractorCheckFunc)(void * , uchar * , uchar * , int );

class PIP_EXPORT PIPacketExtractor: public PIIODevice
{
	PIIODEVICE(PIPacketExtractor)
	friend class PIConnection;
public:

	//! Extract algorithms
	enum SplitMode {
		None /** No data processing */ ,
		Header /** Detect packets with \a header() and following \a payloadSize() */ ,
		Footer /** Detect packets with \a footer() and leading \a payloadSize() */ ,
		HeaderAndFooter /** Detect packets with \a header() and \a footer() without \a payloadSize() */ ,
		Size /** Detect packets with \a packetSize() */ ,
		Timeout /** Wait for first read, then read for \a timeout() milliseconds */
	};

	//! Contructs extractor with child device "device_" and extract algorithm "mode"
	PIPacketExtractor(PIIODevice * device_ = 0, SplitMode mode = None);

	virtual ~PIPacketExtractor() {stop();}


	//! Returns child %device
	PIIODevice * device() {return dev;}

	//! Set child %device to "device_"
	void setDevice(PIIODevice * device_);


	//! Returns buffer size
	int bufferSize() const {return buffer_size;}

	//! Set buffer size to "new_size" bytes, should be at least greater than whole packet size
	void setBufferSize(int new_size) {buffer_size = new_size; buffer.resize(buffer_size); memset(buffer.data(), 0, buffer.size());}

	void setHeaderCheckSlot(PacketExtractorCheckFunc f) {ret_func_header = f;}
	void setFooterCheckSlot(PacketExtractorCheckFunc f) {ret_func_footer = f;}
	void setPayloadCheckSlot(ReadRetFunc f) {ret_func_ = f;}


	//! Set extract algorithm
	void setSplitMode(SplitMode mode) {setProperty("splitMode", int(mode)); mode_ = mode;}

	//! Set payload size, used for PIPacketExtractor::Header and PIPacketExtractor::Footer algorithms
	void setPayloadSize(int size);

	//! Set header data, used for PIPacketExtractor::Header and PIPacketExtractor::HeaderAndFooter algorithms
	void setHeader(const PIByteArray & data);

	//! Set footer data, used for PIPacketExtractor::Footer and PIPacketExtractor::HeaderAndFooter algorithms
	void setFooter(const PIByteArray & data);

	//! Set packet size, used for PIPacketExtractor::Size algorithm
	void setPacketSize(int size) {setProperty("packetSize", size); packetSize_ = size;}

	//! Set timeout in milliseconds, used for PIPacketExtractor::Timeout algorithm
	void setTimeout(double msecs) {setProperty("timeout", msecs); time_ = msecs;}


	//! Returns current extract algorithm
	SplitMode splitMode() const {return (SplitMode)(property("splitMode").toInt());}

	//! Returns current payload size, used for PIPacketExtractor::Header and PIPacketExtractor::Footer algorithms
	int payloadSize() const {return property("payloadSize").toInt();}

	//! Returns current header data, used for PIPacketExtractor::Header and PIPacketExtractor::HeaderAndFooter algorithms
	PIByteArray header() const {return src_header;}

	//! Returns current footer data, used for PIPacketExtractor::Footer and PIPacketExtractor::HeaderAndFooter algorithms
	PIByteArray footer() const {return src_footer;}

	//! Returns current packet size, used for PIPacketExtractor::Size algorithm
	int packetSize() const {return property("packetSize").toInt();}

	//! Returns current timeout in milliseconds, used for PIPacketExtractor::Timeout algorithm
	double timeout() const {return property("timeout").toDouble();}


	//! Returns missed by validating functions bytes count
	ullong missedBytes() const {return missed;}

	// //! Returns missed by validating functions packets count, = missedBytes() / packetSize
	ullong missedPackets() const {/*if (packetSize_hf == 0) return missed; return missed / packetSize_hf*/; return missed_packets;}

	//! Returns pointer to \a missedBytes() count. Useful for output to PIConsole
	const ullong * missedBytes_ptr() const {return &missed;}

	// //! Returns pointer to \a missedPackets() count. Useful for output to PIConsole
	const ullong * missedPackets_ptr() const {return &missed_packets;}

	EVENT2(packetReceived, uchar * , data, int, size)

//! \events
//! \{

	//! \fn void packetReceived(uchar * data, int size)
	//! \brief Raise on successfull \a packetValidate() function

//! \}

protected:

	/** \brief Function to validate header
	 * \param src Your header content
	 * \param rec Received header
	 * \param size Header size
	 * \details Default implementation returns by-byte "src" with "rec" compare result */
	virtual bool validateHeader(uchar * src, uchar * rec, int size) {if (ret_func_header != 0) return ret_func_header(ret_data_, src, rec, size); for (int i = 0; i < size; ++i) if (src[i] != rec[i]) return false; return true;}

	/** \brief Function to validate footer
	 * \param src Your footer content
	 * \param rec Received footer
	 * \param size Footer size
	 * \details Default implementation returns by-byte "src" with "rec" compare result */
	virtual bool validateFooter(uchar * src, uchar * rec, int size) {if (ret_func_footer != 0) return ret_func_footer(ret_data_, src, rec, size); for (int i = 0; i < size; ++i) if (src[i] != rec[i]) return false; return true;}

	/** \brief Function to validate payload
	 * \param rec Received payload
	 * \param size payload size
	 * \details Default implementation returns \b true */
	virtual bool validatePayload(uchar * rec, int size) {if (ret_func_ != 0) return ret_func_(ret_data_, rec, size); return true;}

private:
	void construct();
	void propertyChanged(const PIString & );
	int readDevice(void * read_to, int max_size) {if (dev == 0) return -1; return dev->read(read_to, max_size);}
	int writeDevice(const void * data, int max_size) {if (dev == 0) return -1; return dev->write(data, max_size);}
	bool threadedRead(uchar * readed, int size);
	PIString fullPathPrefix() const {return "pckext";}
	PIString constructFullPathDevice() const;
	bool openDevice() {if (dev == 0) return false; return dev->open();}

	PIIODevice * dev;
	PIByteArray buffer, tmpbuf, src_header, src_footer, trbuf;
	PacketExtractorCheckFunc ret_func_header, ret_func_footer;
	SplitMode mode_;
	int buffer_size, dataSize, packetSize_hf, footerInd, packetSize_;
	double time_;
	bool header_found;
	ullong missed, missed_packets;


};

#endif // PIPACKETEXTRACTOR_H
