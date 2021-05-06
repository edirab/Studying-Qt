/*! \file pibinarylog.h
 * \brief Binary log
*/
/*
	PIP - Platform Independent Primitives
	Class for write binary data to logfile, and read or playback this data
	Copyright (C) 2016  Andrey Bychkov work.a.b@yandex.ru

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

#ifndef PIBINARYLOG_H
#define PIBINARYLOG_H

#include "pifile.h"

#define PIBINARYLOG_VERSION 0x31
namespace __S__PIBinaryLog {
static const uchar binlog_sig[] = {'B','I','N','L','O','G'};
}
#define PIBINARYLOG_SIGNATURE_SIZE sizeof(__S__PIBinaryLog::binlog_sig)

/// TODO: Create static functions to split and join binlog files
/// TODO: Create functions to insert and delete records
class PIBinaryLog: public PIIODevice
{
	PIIODEVICE(PIBinaryLog)
public:
	PIBinaryLog();
	~PIBinaryLog() {closeDevice();}

	//! \brief Play modes for \a PIBinaryLog
	enum PlayMode {
		PlayRealTime       /*! Play in system realtime, default mode */ ,
		PlayVariableSpeed  /*! Play in software realtime with speed, set by \a setSpeed */ ,
		PlayStaticDelay    /*! Play with custom static delay, ignoring timestamp */
	};
	
	//! \brief Different split modes for writing \a PIBinaryLog, which can separate files by size, by time or by records count
	enum SplitMode {
		SplitNone   /*! Without separate, default mode */ ,
		SplitTime   /*! Separate files by record time */ ,
		SplitSize   /*! Separate files by size */ ,
		SplitCount  /*! Separate files by records count */
	};

	//! \brief Struct contains information about all records with same ID
	struct BinLogRecordInfo {
		BinLogRecordInfo() {
			id = count = 0;
			minimum_size = maximum_size = 0;
		}
		int id;
		int count;
		int minimum_size;
		int maximum_size;
		PISystemTime start_time;
		PISystemTime end_time;
	};

	//! \brief Struct contains full information about Binary Log file and about all Records using map of \a BinLogRecordInfo
	struct BinLogInfo {
		PIString path;
		int records_count;
		llong log_size;
		PISystemTime start_time;
		PISystemTime end_time;
		PIMap<int, BinLogRecordInfo> records;
	};

	//! \brief Struct contains position, ID and timestamp of record in file
	struct BinLogIndex {
		int id;
		llong pos;
		PISystemTime timestamp;
	};


	//! Current \a PlayMode
	PlayMode playMode() const {return play_mode;}

	//! Current \a SplitMode
	SplitMode splitMode() const {return split_mode;}

	//! Current directory where billogs wiil be saved
	PIString logDir() const {return property("logDir").toString();}
	
	//! Returns current file prefix
	PIString filePrefix() const {return property("filePrefix").toString();}
	
	//! Default ID, used in \a write function
	int defaultID() const {return default_id;}
	
	//! Returns current play speed
	double playSpeed() const {return play_speed > 0 ? 1. / play_speed : 0.;}

	//! Returns current play delay
	PISystemTime playDelay() const {return play_delay;}

	//! Returns current binlog file split time
	PISystemTime splitTime() const {return split_time;}

	//! Returns current binlog file split size
	llong splitFileSize() const {return split_size;}

	//! Returns current binlog file split records count
	int splitRecordCount() const {return split_count;}

	//! Returns if rapid start enabled
	bool rapidStart() const {return rapid_start;}
	
	//! Create binlog file with Filename = path
	void createNewFile(const PIString &path);
	
	//! Set \a PlayMode
	void setPlayMode(PlayMode mode) {setProperty("playMode", (int)mode);}

	//! Set \a SplitMode
	void setSplitMode(SplitMode mode) {setProperty("splitMode", (int)mode);}

	//! Set path to directory where binlogs will be saved
	void setLogDir(const PIString & path) {setProperty("logDir", path);}
	
	//! Set file prefix, used to
	void setFilePrefix(const PIString & prefix) {setProperty("filePrefix", prefix);}
	
	//! Set defaultID, used in \a write function
	void setDefaultID(int id) {setProperty("defaultID", id);}
	
	//! If enabled BinLog \a ThreadedRead starts without delay for first record, i.e. first record will be readed immediately
	void setRapidStart(bool enabled) {setProperty("rapidStart", enabled);}

	//! Set play speed to "speed", default value is 1.0x
	//! Also this function set \a playMode to \a PlayVariableSpeed
	void setPlaySpeed(double speed) {setPlayMode(PlayVariableSpeed); setProperty("playSpeed", speed);}

	//! Setting static delay between records, default value is 1 sec
	//! Also this function set \a playMode to \a PlayStaticDelay
	void setPlayDelay(const PISystemTime & delay) {setPlayMode(PlayStaticDelay); setProperty("playDelay", delay);}

	//! Set \a playMode to \a PlayRealTime
	void setPlayRealTime() {setPlayMode(PlayRealTime);}

	//! Set binlog file split time
	//! Also this function set \a splitMode to \a SplitTime
	void setSplitTime(const PISystemTime & time) {setSplitMode(SplitTime); setProperty("splitTime", time);}

	//! Set binlog file split size
	//! Also this function set \a splitMode to \a SplitSize
	void setSplitFileSize(llong size) {setSplitMode(SplitSize); setProperty("splitFileSize", size);}

	//! Set binlog file split records count
	//! Also this function set \a splitMode to \a SplitCount
	void setSplitRecordCount(int count) {setSplitMode(SplitCount); setProperty("splitRecordCount", count);}

	//! Set pause while playing via \a threadedRead or writing via write
	void setPause(bool pause);

	//! Write one record to BinLog file, with ID = id, id must be greather than 0
	int writeBinLog(int id, PIByteArray data) {return writeBinLog(id, data.data(), data.size_s());}
	
	//! Write one record to BinLog file, with ID = id, id must be greather than 0
	int writeBinLog(int id, const void * data, int size);

	//! Write one RAW record to BinLog file, with ID = id, Timestamp = time
	int writeBinLog_raw(int id, const PISystemTime &time, const PIByteArray &data) {return writeBinLog_raw(id, time, data.data(), data.size_s());}
	int writeBinLog_raw(int id, const PISystemTime &time, const void * data, int size);

	//! Returns count of writed records
	int writeCount() const {return write_count;}
	
	//! Read one record from BinLog file, with ID = id, if id = 0 than any id will be readed
	PIByteArray readBinLog(int id = 0, PISystemTime * time = 0);
	
	//! Read one record from BinLog file, with ID = id, if id = 0 than any id will be readed
	int readBinLog(int id, void * read_to, int max_size, PISystemTime * time = 0);
	
	//! Returns binary log file size
	llong logSize() const {return file.size();}

	//! Return true, if position at the end of BinLog file
	bool isEnd() const {if (isClosed()) return true; return file.isEnd();}
	
	//! Returns if BinLog file is empty
	bool isEmpty() const {return (file.size() <= PIBINARYLOG_SIGNATURE_SIZE + 1);}

	//! Returns BinLog pause status
	bool isPause() const {return is_pause;}
	
	//! Returns if BinLog file is empty
	int lastReadedID() const {return lastrecord.id;}
	
#ifdef DOXYGEN
	//! Read one message from binlog file, with ID contains in "filterID" or any ID, if "filterID" is empty
	int read(void *read_to, int max_size);
	
	//! Write one record to BinLog file, with ID = "defaultID"
	int write(const void * data, int size);
#endif

	//! Array of ID, that BinLog can read from binlog file, when use \a read function, or in \a ThreadedRead
	PIVector<int> filterID;
	
	//! Go to begin of BinLog file
	void restart();

	//! Get binlog info \a BinLogInfo
	BinLogInfo logInfo() const {if (is_indexed) return binfo; return getLogInfo(path());}

	//! Get binlog index \a BinLogIndex, need \a createIndex before getting index
	const PIVector<BinLogIndex> & logIndex() const {return index;} /// TODO: Think about index positions
	
	//! Create index of current binlog file
	bool createIndex();

	//! Go to record #index
	void seekTo(int rindex);

	//! Go to nearest record
	bool seek(const PISystemTime & time);

	//! Set position in file to reading/playing
	bool seek(llong filepos);

	//! Get current record index (position record in file)
	int pos() const {if (is_indexed) return current_index; return -1;} /// TODO: Think about index positions

	//! \handlers
	//! \{
	
	//! \fn PIString createNewFile()
	//! \brief Create new binlog file in \a logDir, if successful returns filename, else returns empty string.
	//! Filename is like  \a filePrefix + "yyyy_MM_dd__hh_mm_ss.binlog"
	
	//! \}
	//! \events
	//! \{
	
	//! \fn void fileEnd()
	//! \brief Raise on file end while reading
	
	//! \fn void fileError()
	//! \brief Raise on file creation error

	//! \fn void newFile(const PIString & filename)
	//! \brief Raise on new file created

	//! \}
	
	EVENT_HANDLER(PIString, createNewFile);
	EVENT(fileEnd)
	EVENT(fileError)
	EVENT1(newFile, const PIString &, filename)
	EVENT1(posChanged, int, pos) /// TODO: Think about index positions

	//! Get binlog info and statistic
	static BinLogInfo getLogInfo(const PIString & path);

protected:
	PIString fullPathPrefix() const {return "binlog";}
	PIString constructFullPathDevice() const;
	void configureFromFullPathDevice(const PIString & full_path);
	int readDevice(void *read_to, int max_size);
	int writeDevice(const void * data, int size) {return writeBinLog(default_id, data, size);}
	bool openDevice();
	bool closeDevice();
	void propertyChanged(const PIString &);
	bool threadedRead(uchar *readed, int size);

private:
	struct BinLogRecord {
		int id;
		int size;
		PISystemTime timestamp;
		PIByteArray data;
	};

	bool writeFileHeader();
	bool checkFileHeader();
	BinLogRecord readRecord();
	static void parseLog(PIFile *f, BinLogInfo *info, PIVector<BinLogIndex> * index);
	void moveIndex(int i);
	PIString getLogfilePath() const;

	PIVector<BinLogIndex> index;
	PIMap<llong, int> index_pos;
	BinLogInfo binfo;

	PlayMode play_mode;
	SplitMode split_mode;
	PIFile file;
	BinLogRecord lastrecord;
	PISystemTime startlogtime, play_delay, split_time, pause_time;
	PIMutex logmutex, pausemutex;
	double play_time, play_speed;
	llong split_size;
	int write_count, split_count, default_id, current_index;
	bool is_started, is_thread_ok, is_indexed, rapid_start, is_pause;
};

//! \relatesalso PICout \relatesalso PIBinaryLog::BinLogInfo \brief Output operator to PICout
inline PICout operator <<(PICout s, const PIBinaryLog::BinLogInfo & bi) {
	s.space();
	s.setControl(0, true);
	s << "[PIBinaryLog] " << bi.path << "\n";
	if (bi.log_size < 0) {
		s << "invalid file path";
		s.restoreControl();
		return s;
	}
	if (bi.log_size == 0) {
		s << "Invalid empty file";
		s.restoreControl();
		return s;
	}	if (bi.records_count < 0 && bi.records_count > -4) {
		s << "Invalid file or corrupted signature";
		s.restoreControl();
		return s;
	}
	if (bi.records_count < -3) {
		s << "Invalid binlog version";
		s.restoreControl();
		return s;
	}
	s << "read records " << bi.records_count << " in " << bi.records.size() << " types, log size " << bi.log_size;
	s << "\nlog start " << bi.start_time << " , log end " << bi.end_time;
	PIVector<int> keys = bi.records.keys();
	piForeachC(int i, keys) {
		const PIBinaryLog::BinLogRecordInfo &bri(bi.records[i]);
		s << "\n  record id " << bri.id << " , count " << bri.count;
		s << "\n    record start " << bri.start_time << " , end " << bri.end_time;
		s << "\n    record size " << bri.minimum_size << " - " << bri.maximum_size;
	}
	s.restoreControl();
	return s;
}

#endif // PIBINARYLOG_H
