/*! \file pifile.h
 * \brief Local file
*/
/*
    PIP - Platform Independent Primitives
    File
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

#ifndef PIFILE_H
#define PIFILE_H

#include "piiodevice.h"


class PIP_EXPORT PIFile: public PIIODevice
{
	PIIODEVICE(PIFile)
public:
	
	//! Constructs an empty file
	explicit PIFile();
	
	struct FileInfo {
		FileInfo() {size = 0; id_group = id_user = 0;}
		
		enum Flag {
			File         = 0x01,
			Dir          = 0x02,
			Dot          = 0x04,
			DotDot       = 0x08,
			SymbolicLink = 0x10,
			Hidden       = 0x20
		};
		typedef PIFlags<FileInfo::Flag> Flags;
		struct Permissions {
			Permissions(uchar r = 0): raw(r) {}
			Permissions(bool r, bool w, bool e): raw(0) {read = r; write = w; exec = e;}
			PIString toString() const {return PIString(read ? "r" : "-") + PIString(write ? "w" : "-") + PIString(exec ? "x" : "-");}
			operator int() const {return raw;}
			Permissions & operator =(int v) {raw = v; return *this;}
			union {
				uchar raw;
				struct {
					uchar read : 1;
					uchar write: 1;
					uchar exec : 1;
				};
			};
		};
		
		PIString path;
		llong size;
		PIDateTime time_access;
		PIDateTime time_modification;
		Flags flags;
		uint id_user;
		uint id_group;
		Permissions perm_user;
		Permissions perm_group;
		Permissions perm_other;
		
		PIString name() const;
		PIString baseName() const;
		PIString extension() const;
		PIString dir() const;
		bool isDir() const {return flags[Dir];}
		bool isFile() const {return flags[File];}
		bool isSymbolicLink() const {return flags[SymbolicLink];}
		bool isHidden() const {return flags[Hidden];}
	};
	
	//! Constructs a file with path "path" and open mode "mode"
	explicit PIFile(const PIString & path, DeviceMode mode = ReadWrite);
	
	PIFile(const PIFile & other);
	
	~PIFile() {closeDevice();}
	
	//PIFile & operator =(const PIFile & f) {path_ = f.path_; type_ = f.type_; return *this;}
	
	
	//! Immediate write all buffered data to disk
	void flush();
	
	//! Move read/write position to "position"
	void seek(llong position);
	
	//! Move read/write position to the begin of the file
	void seekToBegin();
	
	//! Move read/write position to the end of the file
	void seekToEnd();
	
	//! Move read/write position to text line number "line"
	void seekToLine(llong line);
	//void fill(char c) {stream.fill(c);}
	
	//! Read one char and return it
	char readChar();
	
	//! Read one text line and return it
	PIString readLine();
	
	//! Read all file content to "data" and return readed bytes count. Position leaved unchanged
	llong readAll(void * data);
	
	//! Read all file content to byte array and return it. Position leaved unchanged
	PIByteArray readAll(bool forceRead = false);
	
	
	//! Set file path to "path" and reopen file if need
	void setPath(const PIString & path);
	
	//! Returns file size
	llong size() const;
	
	//! Returns read/write position
	llong pos() const;
	
	//! Returns if position is at the end of file
	bool isEnd() const;
	
	//! Returns if file is empty
	bool isEmpty() const {return (size() <= 0);}
	
	//! Returns FileInfo of current file
	FileInfo fileInfo() const {return fileInfo(path());}
	
	
	//! Returns float numbers write precision
	int precision() const {return prec_;}
	
	//! Set float numbers write precision to "prec_" digits
	void setPrecision(int prec);
	
	
	PIFile & writeToBinLog(ushort id, const void * data, int size); /// DEPRECATED
	
	
	//! Write to file binary content of "v"
	PIFile & writeBinary(const char v) {write(&v, sizeof(v)); return *this;}
	//! Write to file binary content of "v"
	PIFile & writeBinary(const short v) {write(&v, sizeof(v)); return *this;}
	//! Write to file binary content of "v"
	PIFile & writeBinary(const int v) {write(&v, sizeof(v)); return *this;}
	//! Write to file binary content of "v"
	PIFile & writeBinary(const long v) {write(&v, sizeof(v)); return *this;}
	//! Write to file binary content of "v"
	PIFile & writeBinary(const llong v) {write(&v, sizeof(v)); return *this;}
	//! Write to file binary content of "v"
	PIFile & writeBinary(const uchar v) {write(&v, sizeof(v)); return *this;}
	//! Write to file binary content of "v"
	PIFile & writeBinary(const ushort v) {write(&v, sizeof(v)); return *this;}
	//! Write to file binary content of "v"
	PIFile & writeBinary(const uint v) {write(&v, sizeof(v)); return *this;}
	//! Write to file binary content of "v"
	PIFile & writeBinary(const ulong v) {write(&v, sizeof(v)); return *this;}
	//! Write to file binary content of "v"
	PIFile & writeBinary(const ullong v) {write(&v, sizeof(v)); return *this;}
	//! Write to file binary content of "v"
	PIFile & writeBinary(const float v) {write(&v, sizeof(v)); return *this;}
	//! Write to file binary content of "v"
	PIFile & writeBinary(const double v) {write(&v, sizeof(v)); return *this;}
	
	PIFile & operator =(const PIFile & f) {PIIODevice::setPath(f.path()); mode_ = f.mode_; return *this;}
	
	//! Write to file text representation of "v"
	PIFile & operator <<(const char v) {if (canWrite() && fd != 0) write(&v, 1); return *this;}
	//PIFile & operator <<(const string & v) {write(v.c_str(), v.size()); return *this;}
	//! Write to file string "v"
	PIFile & operator <<(const PIString & v);
	//! Write to file text representation of "v"
	PIFile & operator <<(const PIByteArray & v) {if (canWrite() && fd != 0) write(v.data(), v.size()); return *this;}
	//! Write to file text representation of "v"
	PIFile & operator <<(short v) {if (canWrite() && fd != 0) ret = fprintf(fd, "%hd", v); return *this;}
	//! Write to file text representation of "v"
	PIFile & operator <<(int v) {if (canWrite() && fd != 0) ret = fprintf(fd, "%d", v); return *this;}
	//! Write to file text representation of "v"
	PIFile & operator <<(long v) {if (canWrite() && fd != 0) ret = fprintf(fd, "%ld", v); return *this;}
	//! Write to file text representation of "v"
	PIFile & operator <<(llong v) {if (canWrite() && fd != 0) ret = fprintf(fd, "%lld", v); return *this;}
	//! Write to file text representation of "v"
	PIFile & operator <<(uchar v) {if (canWrite() && fd != 0) ret = fprintf(fd, "%u", int(v)); return *this;}
	//! Write to file text representation of "v"
	PIFile & operator <<(ushort v) {if (canWrite() && fd != 0) ret = fprintf(fd, "%hu", v); return *this;}
	//! Write to file text representation of "v"
	PIFile & operator <<(uint v) {if (canWrite() && fd != 0) ret = fprintf(fd, "%u", v); return *this;}
	//! Write to file text representation of "v"
	PIFile & operator <<(ulong v) {if (canWrite() && fd != 0) ret = fprintf(fd, "%lu", v); return *this;}
	//! Write to file text representation of "v"
	PIFile & operator <<(ullong v) {if (canWrite() && fd != 0) ret = fprintf(fd, "%llu", v); return *this;}
	//! Write to file text representation of "v" with precision \a precision()
	PIFile & operator <<(float v) {if (canWrite() && fd != 0) ret = fprintf(fd, ("%" + prec_str + "f").data(), v); return *this;}
	//! Write to file text representation of "v" with precision \a precision()
	PIFile & operator <<(double v) {if (canWrite() && fd != 0) ret = fprintf(fd, ("%" + prec_str + "lf").data(), v); return *this;}
	
	
	//! Read from file text representation of "v"
	PIFile & operator >>(char & v) {if (canRead() && fd != 0) ret = fscanf(fd, "%hhn", &v); return *this;}
	//! Read from file text representation of "v"
	PIFile & operator >>(short & v) {if (canRead() && fd != 0) ret = fscanf(fd, "%hn", &v); return *this;}
	//! Read from file text representation of "v"
	PIFile & operator >>(int & v) {if (canRead() && fd != 0) ret = fscanf(fd, "%n", &v); return *this;}
	//! Read from file text representation of "v"
	PIFile & operator >>(long & v) {if (canRead() && fd != 0) ret = fscanf(fd, "%ln", &v); return *this;}
	//! Read from file text representation of "v"
	PIFile & operator >>(llong & v) {if (canRead() && fd != 0) ret = fscanf(fd, "%lln", &v); return *this;}
	//! Read from file text representation of "v"
	PIFile & operator >>(uchar & v) {if (canRead() && fd != 0) ret = fscanf(fd, "%hhn", &v); return *this;}
	//! Read from file text representation of "v"
	PIFile & operator >>(ushort & v) {if (canRead() && fd != 0) ret = fscanf(fd, "%hn", &v); return *this;}
	//! Read from file text representation of "v"
	PIFile & operator >>(uint & v) {if (canRead() && fd != 0) ret = fscanf(fd, "%n", &v); return *this;}
	//! Read from file text representation of "v"
	PIFile & operator >>(ulong & v) {if (canRead() && fd != 0) ret = fscanf(fd, "%ln", &v); return *this;}
	//! Read from file text representation of "v"
	PIFile & operator >>(ullong & v) {if (canRead() && fd != 0) ret = fscanf(fd, "%lln", &v); return *this;}
	//! Read from file text representation of "v"
	PIFile & operator >>(float & v) {if (canRead() && fd != 0) ret = fscanf(fd, "%f", &v); return *this;}
	//! Read from file text representation of "v"
	PIFile & operator >>(double & v) {if (canRead() && fd != 0) ret = fscanf(fd, "%lf", &v); return *this;}
	
	EVENT_HANDLER(void, clear);
	EVENT_HANDLER(void, remove);
	EVENT_HANDLER1(void, resize, llong, new_size) {resize(new_size, 0);}
	EVENT_HANDLER2(void, resize, llong, new_size, uchar, fill);
	
	//!
	static const char * defaultCharset();

	//!
	static void setDefaultCharset(const char * c);

	//! Returns opened temporary file with open mode "mode"
	static PIFile openTemporary(PIIODevice::DeviceMode mode = PIIODevice::ReadWrite) {return PIFile(PIString(tmpnam(0)), mode);}
	
	//! Returns if file with path "path" does exists
	static bool isExists(const PIString & path);
	
	//! Remove file with path "path" and returns if remove was successful
	static bool remove(const PIString & path);
	
	//! Rename file with path "from" to path "to" and returns if rename was successful
	static bool rename(const PIString & from, const PIString & to) {return ::rename(from.data(), to.data()) == 0;}
	
	//! Returns FileInfo of file or dir with path "path"
	static FileInfo fileInfo(const PIString & path);
	
	//! Apply "info" parameters to file or dir with path "path"
	static bool applyFileInfo(const PIString & path, const FileInfo & info);
	
	//! Apply "info" parameters to file or dir with path "info".path
	static bool applyFileInfo(const FileInfo & info) {return applyFileInfo(info.path, info);}
	
//! \handlers
//! \{

	//! \fn void clear()
	//! \brief Clear content of file
	
	//! \fn void resize(llong new_size)
	//! \brief Resize file to "new_size" with "fill" filling
	
	//! \fn void resize(llong new_size, uchar fill)
	//! \brief Resize file to "new_size" with "fill" filling

	//! \fn void remove()
	//! \brief Remove file

//! \}
//! \ioparams
//! \{
#ifdef DOXYGEN
#endif
//! \}

protected:
	PIString fullPathPrefix() const {return "file";}
	PIString constructFullPathDevice() const;
	void configureFromFullPathDevice(const PIString & full_path);
	int readDevice(void * read_to, int max_size);
	int writeDevice(const void * data, int max_size);
	bool openDevice();
	bool closeDevice();

private:
	PIString strType(const PIIODevice::DeviceMode type);

	FILE * fd;
	int ret, prec_, fdi;
	PIString prec_str;

};

inline PICout operator <<(PICout s, const PIFile::FileInfo & v) {
	s.setControl(0, true);
	s << "FileInfo(\"" << v.path << "\", " << PIString::readableSize(v.size) << ", "
		<< v.perm_user.toString() << " " << v.perm_group.toString() << " " << v.perm_other.toString() << ", "
		<< v.time_access.toString() << ", " << v.time_modification.toString()
		<< ", 0x" << PICoutManipulators::Hex << v.flags << ")";
	s.restoreControl();
	return s;
}


inline PIByteArray & operator <<(PIByteArray & s, const PIFile::FileInfo & v) {s << v.path << v.size << v.time_access << v.time_modification <<
	(int)v.flags << v.id_user << v.id_group << v.perm_user.raw << v.perm_group.raw << v.perm_other.raw; return s;}
inline PIByteArray & operator >>(PIByteArray & s, PIFile::FileInfo & v) {s >> v.path >> v.size >> v.time_access >> v.time_modification >>
	*(int*)(&(v.flags)) >> v.id_user >> v.id_group >> v.perm_user.raw >> v.perm_group.raw >> v.perm_other.raw; return s;}

#endif // PIFILE_H
