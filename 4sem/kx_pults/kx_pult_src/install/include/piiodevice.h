/*! \file piiodevice.h
 * \brief Abstract input/output device
*/
/*
    PIP - Platform Independent Primitives
    Abstract input/output device
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

#ifndef PIIODEVICE_H
#define PIIODEVICE_H

#include "piinit.h"
#include "picollection.h"
#include "pivariant.h"
#include "pitimer.h"

// function executed from threaded read, pass ThreadedReadData, readedData, sizeOfData
typedef bool (*ReadRetFunc)(void * , uchar * , int );

#ifdef DOXYGEN

//! \relatesalso PIIODevice \brief Use this macro to enable automatic creation instances of your class with \a createFromFullPath() function
#  define REGISTER_DEVICE(class)

//! \relatesalso PIIODevice \brief Use this macro instead of PIOBJECT when describe your own PIIODevice
#  define PIIODEVICE(class)

#else

#  define REGISTER_DEVICE(class) ADD_NEW_TO_COLLECTION_WITH_NAME(__PIIODevices__, class, __S__collection_##class##__)
#  define PIIODEVICE(class) PIOBJECT_SUBCLASS(class, PIIODevice) PIIODevice * copy() const {return new class();}

#endif


class PIP_EXPORT PIIODevice: public PIThread
{
	PIOBJECT_SUBCLASS(PIIODevice, PIThread)
	friend void __DevicePool_threadReadDP(void * ddp);
public:
	
	//! Constructs a empty PIIODevice
	PIIODevice();

	//! \brief Open modes for PIIODevice
	enum DeviceMode {
		ReadOnly /*! Device can only read */ = 0x01,
		WriteOnly /*! Device can only write */ = 0x02,
		ReadWrite /*! Device can both read and write */ = 0x03
	};

	//! \brief Options for PIIODevice, works with some devices
	enum DeviceOption {
		BlockingRead /*! \a read block until data is received, default off */ = 0x01,
		BlockingWrite /*! \a write block until data is sent, default off */ = 0x02
	};

	typedef PIFlags<DeviceOption> DeviceOptions;

	PIIODevice(const PIString & path, DeviceMode mode = ReadWrite);
	virtual ~PIIODevice();

	//! Current open mode of device
	DeviceMode mode() const {return mode_;}
	
	//! Set open mode of device
	void setMode(DeviceMode m) {mode_ = m;}

	//! Current device options
	DeviceOptions options() const {return options_;}

	//! Current device option "o" state
	bool isOptionSet(DeviceOption o) const {return options_[o];}

	//! Set device options
	void setOptions(DeviceOptions o);

	//! Set device option "o" to "yes" and return previous state
	bool setOption(DeviceOption o, bool yes = true);

	//! Current path of device
	PIString path() const {return property(PIStringAscii("path")).toString();}
	
	//! Set path of device
	void setPath(const PIString & path) {setProperty(PIStringAscii("path"), path);}
	
	//! Return \b true if mode is ReadOnly or ReadWrite
	bool isReadable() const {return (mode_ & ReadOnly);}
	
	//! Return \b true if mode is WriteOnly or ReadWrite
	bool isWriteable() const {return (mode_ & WriteOnly);}
	
	//! Return \b true if device is successfully opened
	bool isOpened() const {return opened_;}
	
	//! Return \b true if device is closed
	bool isClosed() const {return !opened_;}
	
	//! Return \b true if device can read \b now
	virtual bool canRead() const {return opened_ && (mode_ & ReadOnly);}
	
	//! Return \b true if device can write \b now
	virtual bool canWrite() const {return opened_ && (mode_ & WriteOnly);}


	//! Set execution of \a open enabled while threaded read on closed device
	void setReopenEnabled(bool yes = true) {setProperty(PIStringAscii("reopenEnabled"), yes);}
	
	//! Set timeout in milliseconds between \a open tryings if reopen is enabled
	void setReopenTimeout(int msecs) {setProperty(PIStringAscii("reopenTimeout"), msecs);}

	
	//! Return reopen enable
	bool isReopenEnabled() const {return property(PIStringAscii("reopenEnabled")).toBool();}
	
	//! Return reopen timeout
	int reopenTimeout() {return property(PIStringAscii("reopenTimeout")).toInt();}


	/** \brief Set "threaded read slot"
	 * \details Set external static function of threaded read that will be executed
	 * at every successful threaded read. Function should have format
	 * "bool func(void * data, uchar * readed, int size)" */
	void setThreadedReadSlot(ReadRetFunc func) {ret_func_ = func;}
	
	//! Set custom data that will be passed to "threaded read slot"
	void setThreadedReadData(void * d) {ret_data_ = d;}
	
	/** \brief Set size of threaded read buffer
	 * \details Default size is 4096 bytes. If your device can read at single read
	 * more than 4096 bytes you should use this function to adjust buffer size */
	void setThreadedReadBufferSize(int new_size) {buffer_tr.resize(new_size);}

	//! Return size of threaded read buffer
	int threadedReadBufferSize() const {return buffer_tr.size_s();}
	
	//! Return content of threaded read buffer
	const uchar * threadedReadBuffer() const {return buffer_tr.data();}

	//! Return custom data that will be passed to "threaded read slot"
	void * threadedReadData() const {return ret_data_;}

	
	//! Return \b true if threaded read is started
	bool isThreadedRead() const {return isRunning();}
	
	//! Start threaded read
	void startThreadedRead() {if (!isRunning()) PIThread::start();}
	
	//! Start threaded read and assign "threaded read slot" to "func"
	void startThreadedRead(ReadRetFunc func) {ret_func_ = func; if (!isRunning()) PIThread::start();}
	
	//! Stop threaded read
	void stopThreadedRead() {PIThread::terminate();}
	
	
	//! Return \b true if threaded write is started
	bool isThreadedWrite() const {return write_thread.isRunning();}
	
	//! Start threaded write
	void startThreadedWrite() {if (!write_thread.isRunning()) write_thread.startOnce();}
	
	//! Stop threaded write
	void stopThreadedWrite() {write_thread.terminate();}
	
	//! Clear threaded write task queue
	void clearThreadedWriteQueue() {write_thread.lock(); write_queue.clear(); write_thread.unlock();}

	
	//! Start both threaded read and threaded write
	void start() {startThreadedRead(); startThreadedWrite();}
	
	//! Stop both threaded read and threaded write and if "wait" block until both threads are stop
	void stop(bool wait = false) {stopThreadedRead(); stopThreadedWrite(); if (wait) while (write_thread.isRunning() || isRunning()) msleep(1);}


	//! Read from device maximum "max_size" bytes to "read_to"
	int read(void * read_to, int max_size) {return readDevice(read_to, max_size);}

	//! Read from device maximum "max_size" bytes and return them as PIByteArray
	PIByteArray read(int max_size) {buffer_in.resize(max_size); int ret = readDevice(buffer_in.data(), max_size); if (ret < 0) return PIByteArray(); return buffer_in.resized(ret);}

	//! Write maximum "max_size" bytes of "data" to device
	int write(const void * data, int max_size) {return writeDevice(data, max_size);}

	//! Write "data" to device
	int write(const PIByteArray & data) {return writeDevice(data.data(), data.size_s());}

	
	//! Read from device for "timeout_ms" milliseconds and return readed data as PIByteArray. Timeout should to be greater than 0
	PIByteArray readForTime(double timeout_ms);
	

	//! Add task to threaded write queue and return task ID
	ullong writeThreaded(const void * data, int max_size) {return writeThreaded(PIByteArray(data, uint(max_size)));}
	
	//! Add task to threaded write queue and return task ID
	ullong writeThreaded(const PIByteArray & data);
	
	
	//! Configure device from section "section" of file "config_file", if "parent_section" parent section also will be read
	bool configure(const PIString & config_file, const PIString & section, bool parent_section = false);
	
	
	//! Reimplement to construct full unambiguous string prefix. \ref PIIODevice_sec7
	virtual PIString fullPathPrefix() const {return PIString();}
	
	//! Reimplement to construct full unambiguous string, describes this device, default returns \a fullPathPrefix() + "://" + \a path()
	PIString constructFullPath() const;

	//! Reimplement to configure your device with parameters of full unambiguous string. Default implementation does nothing
	void configureFromFullPath(const PIString & full_path);

	//! \brief Try to determine suitable device, create new one, configure it with \a configureFromFullPath() and returns it.
	//! \details To function \a configureFromFullPath() "full_path" passed without \a fullPathPrefix() + "://".
	//! See \ref PIIODevice_sec7
	static PIIODevice * createFromFullPath(const PIString & full_path);
	
	static PIString normalizeFullPath(const PIString & full_path);

	static void splitFullPath(PIString fpwm, PIString * full_path, DeviceMode * mode = 0, DeviceOptions * opts = 0);

	
	EVENT_HANDLER(bool, open) {if (!init_) init(); opened_ = openDevice(); if (opened_) opened(); return opened_;}
	EVENT_HANDLER1(bool, open, const PIString &, _path) {setPath(_path); if (!init_) init(); opened_ = openDevice(); if (opened_) opened(); return opened_;}
	EVENT_HANDLER1(bool, open, const DeviceMode &, _mode) {mode_ = _mode; if (!init_) init(); opened_ = openDevice(); if (opened_) opened(); return opened_;}
	EVENT_HANDLER2(bool, open, const PIString &, _path, const DeviceMode &, _mode) {setPath(_path); mode_ = _mode; if (!init_) init(); opened_ = openDevice(); if (opened_) opened(); return opened_;}
	EVENT_HANDLER(bool, close) {opened_ = !closeDevice(); if (!opened_) closed(); return !opened_;}

	EVENT_VHANDLER(void, flush) {;}
	
	EVENT(opened)
	EVENT(closed)
	EVENT2(threadedReadEvent, uchar * , readed, int, size)
	EVENT2(threadedWriteEvent, ullong, id, int, written_size)

//! \handlers
//! \{

	//! \fn bool open()
	//! \brief Open device
	
	//! \fn bool open(const PIString & path)
	//! \brief Open device with path "path"

	//! \fn bool open(const DeviceMode & mode)
	//! \brief Open device with mode "mode"

	//! \fn bool open(const PIString & path, const DeviceMode & mode)
	//! \brief Open device with path "path" and mode "mode"

	//! \fn bool close()
	//! \brief Close device

//! \}
//! \vhandlers
//! \{

	//! \fn void flush()
	//! \brief Immediate write all buffers
	
//! \}
//! \events
//! \{

	//! \fn void opened()
	//! \brief Raise if succesfull open
	
	//! \fn void closed()
	//! \brief Raise if succesfull close

	//! \fn void threadedReadEvent(uchar * readed, int size)
	//! \brief Raise if read thread succesfull read some data

	//! \fn void threadedWriteEvent(ullong id, int written_size)
	//! \brief Raise if write thread successfull write some data of task with ID "id"
	
//! \}
//! \ioparams
//! \{
#ifdef DOXYGEN
	//! \brief setReopenEnabled, default "true"
	bool reopenEnabled;
	
	//! \brief setReopenTimeout in ms, default 1000
	int reopenTimeout;
	
	//! \brief setThreadedReadBufferSize in bytes, default 4096
	int threadedReadBufferSize;
#endif
//! \}

protected:
	
	//! Function executed before first \a openDevice() or from constructor
	virtual bool init() {return true;}
	
	//! Reimplement to configure device from entries "e_main" and "e_parent", cast arguments to \a PIConfig::Entry*
	virtual bool configureDevice(const void * e_main, const void * e_parent = 0) {return true;}
	
	//! Reimplement to open device, return value will be set to "opened_" variable; don't call this function in subclass, use open()
	virtual bool openDevice() = 0; // use path_, type_, opened_, init_ variables
	
	//! Reimplement to close device, inverse return value will be set to "opened_" variable
	virtual bool closeDevice() {return true;} // use path_, type_, opened_, init_ variables

	//! Reimplement this function to read from your device
	virtual int readDevice(void * read_to, int max_size) {piCoutObj << "\"read\" is not implemented!"; return -2;}

	//! Reimplement this function to write to your device
	virtual int writeDevice(const void * data, int max_size) {piCoutObj << "\"write\" is not implemented!"; return -2;}

	//! Function executed when thread read some data, default implementation execute external slot "ret_func_"
	virtual bool threadedRead(uchar * readed, int size);
	
	//! Reimplement to construct full unambiguous string, describes this device. Default implementation returns \a path()
	virtual PIString constructFullPathDevice() const {return path();}

	//! Reimplement to configure your device with parameters of full unambiguous string. Default implementation does nothing
	virtual void configureFromFullPathDevice(const PIString & full_path) {;}

	//! Reimplement to apply new device options
	virtual void optionsChanged() {;}

	
	void terminate();
	
	
	DeviceMode mode_;
	DeviceOptions options_;
	ReadRetFunc ret_func_;
	bool opened_;
	void * ret_data_;

private:
	EVENT_HANDLER2(void, check_start, void * , data, int, delim);
	EVENT_HANDLER(void, write_func);

	virtual PIIODevice * copy() const {return 0;}
	PIString fullPathOptions() const;
	void _init();
	void begin();
	void run();
	void end() {terminate();}
	static void cacheFullPath(const PIString & full_path, const PIIODevice * d);

	PITimer timer;
	PITimeMeasurer tm;
	PIThread write_thread;
	PIByteArray buffer_in, buffer_tr;
	PIQueue<PIPair<PIByteArray, ullong> > write_queue;
	ullong tri;
	int readed_;
	bool init_, thread_started_, raise_threaded_read_;

	static PIMutex nfp_mutex;
	static PIMap<PIString, PIString> nfp_cache;

};

#endif // PIIODEVICE_H
