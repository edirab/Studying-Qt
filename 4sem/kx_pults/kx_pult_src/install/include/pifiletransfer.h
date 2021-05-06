/*! \file pifiletransfer.h
 * \brief Class for send and receive files and directories via \a PIBaseTransfer
*/
/*
	PIP - Platform Independent Primitives
	Class for send and receive files and directories via PIBaseTransfer
	Copyright (C) 2016 Andrey Bychkov work.a.b@yandex.ru

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

#ifndef PIFILETRANSFER_H
#define PIFILETRANSFER_H

#include "pidir.h"
#include "pibasetransfer.h"

#define __PIFILETRANSFER_VERSION 2

class PIFileTransfer : public PIBaseTransfer
{
	PIOBJECT_SUBCLASS(PIFileTransfer, PIBaseTransfer)
public:
	PIFileTransfer();
	~PIFileTransfer();

	enum StepType {pft_None, pft_Description, pft_Data};
	struct PFTFileInfo : public PIFile::FileInfo {
		PFTFileInfo(const PIFile::FileInfo &fi = PIFile::FileInfo()) : PIFile::FileInfo(fi) {}
		PIString dest_path;
	};

#pragma pack(push,1)
	struct PFTHeader {
		union {
			struct {
				char sig[3]; // PFT
				uchar version;
			};
			uint raw_sig;
		};
		int step; // PacketType
		int session_id;
		bool check_sig() {
			if (sig[0] != sign[0] || sig[1] != sign[1] || sig[2] != sign[2] || version != __PIFILETRANSFER_VERSION) return false;
			return true;
		}
	};
#pragma pack(pop)

	bool send(const PIFile & file);
	bool send(const PIString & file);
	bool send(const PIStringList &files);
	bool send(PIFile::FileInfo entry) {return send(PIVector<PIFile::FileInfo>() << entry);}
	bool send(PIVector<PIFile::FileInfo> entries);

	void setDirectory(const PIDir &d) {dir = d;}
	void setDirectory(const PIString &path) {dir.setDir(path);}
	PIDir directory() const {return dir;}

	bool isStarted() const {return started_;}

	PIString curFile() const;
	llong bytesFileAll() const {return bytes_file_all;}
	llong bytesFileCur() const {return bytes_file_cur;}
	const PIString * curFile_ptr() const {return &cur_file_string;}
	const llong * bytesFileAll_ptr() const {return &bytes_file_all;}
	const llong * bytesFileCur_ptr() const {return &bytes_file_cur;}

	EVENT(receiveFilesStarted)
	EVENT1(receiveFilesFinished, bool, ok)
	EVENT(sendFilesStarted)
	EVENT1(sendFilesFinished, bool, ok)
	EVENT3(receiveFilesRequest, PIStringList, files, llong, total_bytes, bool *, ok)
	
private:
	static const char sign[];
	PIVector<PFTFileInfo> files_;
	PIString cur_file_string;
	llong bytes_file_all, bytes_file_cur;
	PFTHeader pftheader;
	PIDir dir;
	PIFile work_file;
	PIByteArray desc;
	PIDir scan_dir;
	bool started_, scanning;

	bool sendFiles(const PIVector<PFTFileInfo> &files);
	void processFile(int id, ullong start, PIByteArray &data);
	virtual void receivePart(Part fi, PIByteArray ba, PIByteArray pheader);
	virtual PIByteArray buildPacket(Part fi);
	virtual PIByteArray customHeader();
	// 	EVENT_HANDLER(void, send_started);
	EVENT_HANDLER(void, receive_started);
	EVENT_HANDLER1(void, send_finished, bool, ok);
	EVENT_HANDLER1(void, receive_finished, bool, ok);
};

inline PIByteArray & operator <<(PIByteArray & s, const PIFileTransfer::PFTHeader & v) {s << v.raw_sig << v.step << v.session_id; return s;}
inline PIByteArray & operator >>(PIByteArray & s, PIFileTransfer::PFTHeader & v) {s >> v.raw_sig >> v.step >> v.session_id; return s;}

inline PIByteArray & operator <<(PIByteArray & s, const PIFileTransfer::PFTFileInfo & v) {s << v.dest_path << v.size << v.time_access << v.time_modification <<
	(int)v.flags << v.id_user << v.id_group << v.perm_user.raw << v.perm_group.raw << v.perm_other.raw; return s;}
inline PIByteArray & operator >>(PIByteArray & s, PIFileTransfer::PFTFileInfo & v) {s >> v.dest_path >> v.size >> v.time_access >> v.time_modification >>
	*(int*)(&(v.flags)) >> v.id_user >> v.id_group >> v.perm_user.raw >> v.perm_group.raw >> v.perm_other.raw; return s;}

inline PICout operator <<(PICout s, const PIFileTransfer::PFTFileInfo & v) {
	s.setControl(0, true);
	s << "FileInfo(\"" << v.dest_path << "\", " << PIString::readableSize(v.size) << ", "
		<< v.perm_user.toString() << " " << v.perm_group.toString() << " " << v.perm_other.toString() << ", "
		<< v.time_access.toString() << ", " << v.time_modification.toString()
		<< ", 0x" << PICoutManipulators::Hex << v.flags << ")";
	s.restoreControl();
	return s;
}
#endif // PIFILETRANSFER_H
