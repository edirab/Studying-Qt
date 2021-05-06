/*
    PIP - Platform Independent Primitives
    System information
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

#ifndef PISYSTEMINFO_H
#define PISYSTEMINFO_H

#include "pitime.h"

class PIP_EXPORT PISystemInfo {
public:
	PISystemInfo();
	
	struct MountInfo {
		MountInfo() {space_all = space_used = space_free = 0;}
		PIString mount_point;
		PIString device;
		PIString filesystem;
		PIString label;
		ullong space_all;
		ullong space_used;
		ullong space_free;
	};
	
	PIString ifconfigPath, execCommand, hostname, user, OS_name, OS_version, architecture;
	PIDateTime execDateTime;
	int processorsCount;
	
	static PIStringList mountRoots();
	static PIVector<MountInfo> mountInfo();
	
	static PISystemInfo * instance();
	
};

inline PICout operator <<(PICout s, const PISystemInfo::MountInfo & v) {
	s.setControl(0, true);
	s << "MountInfo(" << v.device << " mounted on \"" << v.mount_point << "\", type " << v.filesystem
		<< ", label \"" << v.label << "\", all " << PIString::readableSize(v.space_all)
		<< ", used " << PIString::readableSize(v.space_used)
		<< ", free " << PIString::readableSize(v.space_free) << ")";
	s.restoreControl();
	return s;
}

#endif // PISYSTEMINFO_H
