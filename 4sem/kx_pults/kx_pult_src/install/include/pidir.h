/*! \file pidir.h
 * \brief Local directory
*/
/*
    PIP - Platform Independent Primitives
    Directory
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

#ifndef PIDIR_H
#define PIDIR_H

#include "pifile.h"
//#if !defined(ANDROID)

class PIP_EXPORT PIDir
{
public:
	
	//! Constructs directory with path "path"
	PIDir(const PIString & dir = PIString());
	
	//! Constructs directory with "file" directory path "path"
	PIDir(const PIFile & file);
	
	
	//! Returns if this directory is exists
	bool isExists() const {return PIDir::isExists(path());}
	
	//! Returns if path of this directory is absolute
	bool isAbsolute() const;
	
	//! Returns if path of this directory is relative
	bool isRelative() const {return !isAbsolute();}
	
	//! Returns path of current reading directory. This path
	//! valid only while \a allEntries functions
	const PIString & scanDir() const {return scan_;}
	
	
	//! Returns path of this directory
	PIString path() const;
	
	//! Returns absolute path of this directory
	PIString absolutePath() const;
	
	/** \brief Simplify path of this directory
	 * \details This function remove repeatedly separators and
	 * resolve ".." in path. E.g. "/home/.//peri4/src/../.." will
	 * become "/home" \n This function returns reference to this %PIDir */
	PIDir & cleanPath();
	
	//! Returns %PIDir with simplified path of this directory
	PIDir cleanedPath() const {PIDir d(path()); d.cleanPath(); return d;}
	
	//! Returns relative to this directory path "path"
	PIString relative(const PIString & path) const;
	
	//! Set this directory path to simplified "path"
	PIDir & setDir(const PIString & path);
	
	//! Set this directory path as current for application
	bool setCurrent() {return PIDir::setCurrent(path());}
	
	
	/** \brief Returns this directory content
	 * \details Scan this directory and returns all directories
	 * and files in one list, sorted alphabetically. This list
	 * contains also "." and ".." members. There are absolute
	 * pathes in returned list.
	 * \attention This function doesn`t scan content of inner
	 * directories! */
	PIVector<PIFile::FileInfo> entries();
	
	/** \brief Returns all this directory content
	 * \details Scan this directory recursively and returns all
	 * directories and files in one list, sorted alphabetically.
	 * This list doesn`t contains "." and ".." members. There
	 * are absolute pathes in returned list, and
	 * files placed after directories in this list */
	PIVector<PIFile::FileInfo> allEntries();
	
	bool make(bool withParents = true);
	bool remove() {return PIDir::remove(path());}
	bool rename(const PIString & new_name) {if (!PIDir::rename(path(), new_name)) return false; setDir(new_name); return true;}
	PIDir & cd(const PIString & path);
	PIDir & up() {return cd("..");}
	
	bool operator ==(const PIDir & d) const;
	bool operator !=(const PIDir & d) const {return !((*this) == d);}
	
	static const PIChar separator;
	
	static PIDir current();
	static PIDir home();
	static PIDir temporary();
	static PIVector<PIFile::FileInfo> allEntries(const PIString & path);
	static bool isExists(const PIString & path);
	static bool make(const PIString & path, bool withParents = true);
	static bool remove(const PIString & path) {return removeDir(path);}
	static bool rename(const PIString & path, const PIString & new_name) {return PIDir::renameDir(path, new_name);}
	static bool setCurrent(const PIString & path);
	static bool setCurrent(const PIDir & dir) {return setCurrent(dir.path());}
	
private:
	static bool makeDir(const PIString & path);
	static bool removeDir(const PIString & path);
	static bool renameDir(const PIString & path, const PIString & new_name);
	
	PIString path_, scan_;
	
};


inline bool operator  <(const PIFile::FileInfo & v0, const PIFile::FileInfo & v1) {return (v0.path <  v1.path);}
inline bool operator  >(const PIFile::FileInfo & v0, const PIFile::FileInfo & v1) {return (v0.path >  v1.path);}
inline bool operator ==(const PIFile::FileInfo & v0, const PIFile::FileInfo & v1) {return (v0.path == v1.path);}
inline bool operator !=(const PIFile::FileInfo & v0, const PIFile::FileInfo & v1) {return (v0.path != v1.path);}

inline PICout operator <<(PICout s, const PIDir & v) {s.setControl(0, true); s << "PIDir(\"" << v.path() << "\")"; s.restoreControl(); return s;}


#endif // PIDIR_H
