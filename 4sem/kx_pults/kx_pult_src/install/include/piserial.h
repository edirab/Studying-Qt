/*! \file piserial.h
 * \brief Serial device
*/
/*
    PIP - Platform Independent Primitives
    COM
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

#ifndef PISERIAL_H
#define PISERIAL_H

#include "pitimer.h"
#include "piiodevice.h"

class PIP_EXPORT PISerial: public PIIODevice
{
	PIIODEVICE(PISerial)
public:
	
	//! Contructs an empty %PISerial
	explicit PISerial();

	//! \brief Parameters of PISerial
	enum Parameters {
		ParityControl /*! Enable parity check and generate */ = 0x1,
		ParityOdd /*! Parity is odd instead of even */ = 0x2,
		TwoStopBits /*! Two stop bits instead of one */ = 0x4
	};

	//! \brief Speed of PISerial
	enum Speed {
		S50 /*! 50 baud */ = 50,
		S75 /*! 75 baud */ = 75,
		S110 /*! 110 baud */ = 110,
		S300 /*! 300 baud */ = 300,
		S600 /*! 600 baud */ = 600,
		S1200 /*! 1200 baud */ = 1200,
		S2400 /*! 2400 baud */ = 2400,
		S4800 /*! 4800 baud */ = 4800,
		S9600 /*! 9600 baud */ = 9600,
		S19200 /*! 19200 baud */ = 19200,
		S38400 /*! 38400 baud */ = 38400,
		S57600 /*! 57600 baud */ = 57600,
		S115200 /*! 115200 baud */ = 115200,
		S230400 /*! 230400 baud */ = 230400,
		S460800 /*! 460800 baud */ = 460800,
		S500000 /*! 500000 baud */ = 500000,
		S576000 /*! 576000 baud */ = 576000,
		S921600 /*! 921600 baud */ = 921600,
		S1000000 /*! 1000000 baud */ = 1000000,
		S1152000 /*! 1152000 baud */ = 1152000,
		S1500000 /*! 1500000 baud */ = 1500000,
		S2000000 /*! 2000000 baud */ = 2000000,
		S2500000 /*! 2500000 baud */ = 2500000,
		S3000000 /*! 3000000 baud */ = 3000000,
		S3500000 /*! 3500000 baud */ = 3500000,
		S4000000 /*! 4000000 baud */ = 4000000
	};

	//! Contructs %PISerial with device name "device", speed "speed" and parameters "params"
	explicit PISerial(const PIString & device, PISerial::Speed speed = S115200, PIFlags<PISerial::Parameters> params = 0);
	
	~PISerial();

	
	//! Set both input and output speed to "speed"
	void setSpeed(PISerial::Speed speed) {setProperty("outSpeed", (int)speed); setProperty("inSpeed", (int)speed); applySettings();}
	
	//! Set output speed to "speed"
	void setOutSpeed(PISerial::Speed speed) {setProperty("outSpeed", (int)speed); applySettings();}
	
	//! Set input speed to "speed"
	void setInSpeed(PISerial::Speed speed) {setProperty("inSpeed", (int)speed); applySettings();}
	
	//! Set device name to "dev"
	void setDevice(const PIString & dev) {setPath(dev); if (isOpened()) {close(); open();};}

	
	//! Set parameters to "parameters_"
	void setParameters(PIFlags<PISerial::Parameters> parameters_) {setProperty("parameters", (int)parameters_); applySettings();}
	
	//! Set parameter "parameter" to "on" state
	void setParameter(PISerial::Parameters parameter, bool on = true);
	
	//! Returns if parameter "parameter" is set
	bool isParameterSet(PISerial::Parameters parameter) const;
	
	//! Returns parameters
	PIFlags<PISerial::Parameters> parameters() const {return (PIFlags<Parameters>)(property("parameters").toInt());}

	
	//! Set data bits count. Valid range is from 5 to 8, befault is 8
	void setDataBitsCount(int bits) {setProperty("dataBitsCount", bits); applySettings();}
	
	//! Returns data bits count
	int dataBitsCount() const {return property("dataBitsCount").toInt();}

	
	//! Set pin number "number" to logic level "on". Valid numbers are 4 (DTR) and 7 (RTS)
	bool setPin(int number, bool on);
	
	//! Returns pin number "number" logic level. Valid numbers range is from 1 to 9
	bool isPin(int number) const;

	bool setLE(bool on);  // useless function, just formally
	bool setDTR(bool on);
	bool setRTS(bool on);
	bool setCTS(bool on); // useless function, just formally
	bool setST(bool on);  // useless function, just formally
	bool setSR(bool on);  // useless function, just formally
	bool setCAR(bool on); // useless function, just formally
	bool setRNG(bool on); // useless function, just formally
	bool setDSR(bool on); // useless function, just formally

	bool isLE() const;
	bool isDTR() const;
	bool isRTS() const;
	bool isCTS() const;
	bool isST() const;
	bool isSR() const;
	bool isCAR() const;
	bool isRNG() const;
	bool isDSR() const;

	void setVTime(int t) {vtime = t; applySettings();}


	
	//! Returns device name
	PIString device() const {return path();}
	
	//! Returns output speed
	PISerial::Speed outSpeed() const {return (PISerial::Speed)(property("outSpeed").toInt());}
	
	//! Returns input speed
	PISerial::Speed inSpeed() const {return (PISerial::Speed)(property("inSpeed").toInt());}
	
	int VTime() const {return vtime;}

	
	//! Discard all buffered input and output data
	void flush();
	

	int read(void * read_to, int max_size) {return readDevice(read_to, max_size);}
	bool read(void * read_to, int max_size, double timeout_ms);
	PIString read(int size = -1, double timeout_ms = 1000.);
	PIByteArray readData(int size = -1, double timeout_ms = 1000.);


	//! \brief Write to device data "data" with maximum size "size" and wait for data written if "wait" is \b true.
	//! \returns \b true if sended bytes count = "size"
	bool send(const void * data, int size) {return (write(data, size) == size);}

	/// NOTE: no reason to use this function, use PIString::toUtf8() or PIString::dataAscii(),lengthAscii() instead
	//! \brief Write to device string "data" and wait for data written if "wait" is \b true.
	//! \returns \b true if sended bytes count = size of string
	//bool send(const PIString & data, bool wait = false) {return (write(data.data(), data.lengthAscii(), wait) == data.size_s());}

	//! \brief Write to device byte array "data" and wait for data written if "wait" is \b true.
	//! \returns \b true if sended bytes count = size of string
	bool send(const PIByteArray & data) {return (write(data.data(), data.size_s()) == data.size_s());}
	
	//! \brief Returns all available speeds for serial devices
	static PIVector<int> availableSpeeds();
	
	//! \brief Returns all available system devices. If "test" each device will be tried to open
	static PIStringList availableDevices(bool test = false);

//! \ioparams
//! \{
#ifdef DOXYGEN
	//! \brief device, default ""
	string device;
	
	//! \brief input/output speed, default 115200
	int speed;
	
	//! \brief dataBitsCount, default 8
	int dataBitsCount;
	
	//! \brief parityControl, default false
	bool parityControl;
	
	//! \brief parityOdd, default false
	bool parityOdd;
	
	//! \brief twoStopBits, default false
	bool twoStopBits;
#endif
//! \}
	
protected:
	PIString fullPathPrefix() const {return "ser";}
	PIString constructFullPathDevice() const;
	void configureFromFullPathDevice(const PIString & full_path);
	bool configureDevice(const void * e_main, const void * e_parent = 0);
	void optionsChanged();
	int readDevice(void * read_to, int max_size);
	int writeDevice(const void * data, int max_size);
	
	//! Executes when any read function was successful. Default implementation does nothing
	virtual void received(const void * data, int size) {;}

	void construct();
	void applySettings();
	void setTimeouts();
	int convertSpeed(PISerial::Speed speed);
	bool setBit(int bit, bool on, const PIString & bname);
	bool isBit(int bit, const PIString & bname) const;

	bool openDevice();
	bool closeDevice();

	PRIVATE_DECLARATION
	int fd, vtime;
	bool sending;
	PITimeMeasurer tm_;

};

#endif // PISERIAL_H
