/*! \file piconfig.h
 * \brief Configuration parser and writer
*/
/*
	PIP - Platform Independent Primitives
	Configuration parser and writer
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

#ifndef PICONFIG_H
#define PICONFIG_H

#include "piiodevice.h"

#define PICONFIG_GET_VALUE \
	Entry & getValue(const PIString & vname, const char * def, bool * exists = 0) {return getValue(vname, PIString(def), exists);} \
	Entry & getValue(const PIString & vname, const PIStringList & def, bool * exists = 0) {return getValue(vname, def.join("%|%"), exists);} \
	Entry & getValue(const PIString & vname, const bool def, bool * exists = 0) {return getValue(vname, PIString::fromBool(def), exists);} \
	Entry & getValue(const PIString & vname, const short def, bool * exists = 0) {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const int def, bool * exists = 0) {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const long def, bool * exists = 0) {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const uchar def, bool * exists = 0) {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const ushort def, bool * exists = 0) {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const uint def, bool * exists = 0) {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const ulong def, bool * exists = 0) {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const float def, bool * exists = 0) {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const double def, bool * exists = 0) {return getValue(vname, PIString::fromNumber(def), exists);} \
	\
	Entry & getValue(const PIString & vname, const char * def, bool * exists = 0) const {return getValue(vname, PIString(def), exists);} \
	Entry & getValue(const PIString & vname, const PIStringList & def, bool * exists = 0) const {return getValue(vname, def.join("%|%"), exists);} \
	Entry & getValue(const PIString & vname, const bool def, bool * exists = 0) const {return getValue(vname, PIString::fromBool(def), exists);} \
	Entry & getValue(const PIString & vname, const short def, bool * exists = 0) const {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const int def, bool * exists = 0) const {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const long def, bool * exists = 0) const {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const uchar def, bool * exists = 0) const {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const ushort def, bool * exists = 0) const {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const uint def, bool * exists = 0) const {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const ulong def, bool * exists = 0) const {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const float def, bool * exists = 0) const {return getValue(vname, PIString::fromNumber(def), exists);} \
	Entry & getValue(const PIString & vname, const double def, bool * exists = 0) const {return getValue(vname, PIString::fromNumber(def), exists);}

class PIP_EXPORT PIConfig
{
	friend class Entry;
	friend class Branch;
public:

	//! Contructs and read configuration file at path "path" in mode "mode"
	PIConfig(const PIString & path, PIIODevice::DeviceMode mode = PIIODevice::ReadWrite);

	//! Contructs and read configuration string "string" in mode "mode"
	PIConfig(PIString * string, PIIODevice::DeviceMode mode = PIIODevice::ReadWrite);

	//! Contructs and read configuration from custom device "device" in mode "mode"
	PIConfig(PIIODevice * device = 0, PIIODevice::DeviceMode mode = PIIODevice::ReadWrite);

	~PIConfig();

	class Entry;


	class PIP_EXPORT Branch: public PIVector<Entry * > {
		friend class PIConfig;
		friend class Entry;
		friend std::ostream & operator <<(std::ostream & s, const Branch & v);
		friend PICout operator <<(PICout s, const Branch & v);
	public:
		Branch() {;}

		Entry & getValue(const PIString & vname, const PIString & def = PIString(), bool * exists = 0);
		Entry & getValue(const PIString & vname, const PIString & def = PIString(), bool * exists = 0) const {return const_cast<Branch * >(this)->getValue(vname, def, exists);}
		PICONFIG_GET_VALUE

		Branch allLeaves();
		Branch getValues(const PIString & name);
		Branch getLeaves();
		Branch getBranches();
		Branch & filter(const PIString & f);
		bool isEntryExists(const PIString & name) const {piForeachC (Entry * i, *this) if (entryExists(i, name)) return true; return false;}
		int indexOf(const Entry * e) {for (int i = 0; i < size_s(); ++i) if (at(i) == e) return i; return -1;}

		//void clear() {piForeach (Entry * i, *this) delete i; PIVector<Entry * >::clear();}

	private:
		bool entryExists(const Entry * e, const PIString & name) const;
		void allLeaves(Branch & b, Entry * e) {piForeach (Entry * i, e->_children) {if (i->isLeaf()) b << i; else allLeaves(b, i);}}
		void coutt(std::ostream & s, const PIString & p) const {piForeachC (Entry * i, *this) i->coutt(s, p);}
		void piCoutt(PICout s, const PIString & p) const {piForeachC (Entry * i, *this) i->piCoutt(s, p);}

		static Entry _empty;
		PIString delim;

	};


	class PIP_EXPORT Entry {
		friend class PIConfig;
		friend class Branch;
	public:
		Entry() {_parent = 0; _line = -1;}

		//! Returns parent entry, or 0 if there is no parent (root of default value)
		Entry * parent() const {return _parent;}

		//! Returns children count
		int childCount() const {return _children.size_s();}

		//! Returns children as \a PIConfig::Branch
		Branch & children() const {_children.delim = delim; return _children;}

		//! Returns child at index "index"
		Entry * child(const int index) const {return _children[index];}

		//! Returns first child with name "name"
		Entry * findChild(const PIString & name) {piForeach (Entry * i, _children) if (i->_name == name) return i; return 0;}

		//! Returns first child with name "name"
		const Entry * findChild(const PIString & name) const {piForeachC (Entry * i, _children) if (i->_name == name) return i; return 0;}

		//! Returns \b true if there is no children
		bool isLeaf() const {return _children.isEmpty();}


		//! Returns name
		const PIString & name() const {return _name;}

		//! Returns value
		const PIString & value() const {return _value;}

		//! Returns type
		const PIString & type() const {return _type;}

		//! Returns comment
		const PIString & comment() const {return _comment;}

		/** \brief Returns full name, i.e. name as it looks in file
		 * \details In case of default entry full name always is empty
		 * \snippet piconfig.cpp fullName */
		const PIString & fullName() const {return _full_name;}

		//! Set name to "value" and returns this
		Entry & setName(const PIString & value) {_name = value; return *this;}

		//! Set type to "value" and returns this
		Entry & setType(const PIString & value) {_type = value; return *this;}

		//! Set comment to "value" and returns this
		Entry & setComment(const PIString & value) {_comment = value; return *this;}

		//! Set value to "value" and returns this
		Entry & setValue(const PIString & value) {_value = value; return *this;}

		//! Set value to "value" and returns this. Type is set to "l"
		Entry & setValue(const PIStringList & value) {setValue(value.join("%|%")); setType("l"); return *this;}

		//! Set value to "value" and returns this. Type is set to "s"
		Entry & setValue(const char * value) {setValue(PIString(value)); setType("s"); return *this;}

		//! Set value to "value" and returns this. Type is set to "b"
		Entry & setValue(const bool value) {setValue(PIString::fromBool(value)); setType("b"); return *this;}

		//! Set value to "value" and returns this. Type is set to "s"
		Entry & setValue(const char value) {setValue(PIString(1, value)); setType("s"); return *this;}

		//! Set value to "value" and returns this. Type is set to "n"
		Entry & setValue(const short value) {setValue(PIString::fromNumber(value)); setType("n"); return *this;}

		//! Set value to "value" and returns this. Type is set to "n"
		Entry & setValue(const int value) {setValue(PIString::fromNumber(value)); setType("n"); return *this;}

		//! Set value to "value" and returns this. Type is set to "n"
		Entry & setValue(const long value) {setValue(PIString::fromNumber(value)); setType("n"); return *this;}

		//! Set value to "value" and returns this. Type is set to "n"
		Entry & setValue(const uchar value) {setValue(PIString::fromNumber(value)); setType("n"); return *this;}

		//! Set value to "value" and returns this. Type is set to "n"
		Entry & setValue(const ushort value) {setValue(PIString::fromNumber(value)); setType("n"); return *this;}

		//! Set value to "value" and returns this. Type is set to "n"
		Entry & setValue(const uint value) {setValue(PIString::fromNumber(value)); setType("n"); return *this;}

		//! Set value to "value" and returns this. Type is set to "n"
		Entry & setValue(const ulong value) {setValue(PIString::fromNumber(value)); setType("n"); return *this;}

		//! Set value to "value" and returns this. Type is set to "f"
		Entry & setValue(const float value) {setValue(PIString::fromNumber(value)); setType("f"); return *this;}

		//! Set value to "value" and returns this. Type is set to "f"
		Entry & setValue(const double value) {setValue(PIString::fromNumber(value)); setType("f"); return *this;}


		/** \brief Returns entry with name "vname" and default value "def"
		 * \details If there is no suitable entry found, reference to default internal entry with
		 * value = "def" will be returned, and if "exists" not null it will be set to \b false */
		Entry & getValue(const PIString & vname, const PIString & def = PIString(), bool * exists = 0);
		Entry & getValue(const PIString & vname, const PIString & def = PIString(), bool * exists = 0) const {return const_cast<Entry * >(this)->getValue(vname, def, exists);}
		PICONFIG_GET_VALUE

		//! \fn Entry & getValue(const PIString & vname, const char * def, bool * exists = 0)
		//! \brief Returns entry with name "vname" and default value "def"

		//! \fn Entry & getValue(const PIString & vname, const char * def, bool * exists = 0)
		//! \brief Returns entry with name "vname" and default value "def"

		//! \fn Entry & getValue(const PIString & vname, const PIStringList & def, bool * exists = 0)
		//! \brief Returns entry with name "vname" and default value "def"

		//! \fn Entry & getValue(const PIString & vname, const bool def, bool * exists = 0)
		//! \brief Returns entry with name "vname" and default value "def"

		//! \fn Entry & getValue(const PIString & vname, const short def, bool * exists = 0)
		//! \brief Returns entry with name "vname" and default value "def"

		//! \fn Entry & getValue(const PIString & vname, const int def, bool * exists = 0)
		//! \brief Returns entry with name "vname" and default value "def"

		//! \fn Entry & getValue(const PIString & vname, const long def, bool * exists = 0)
		//! \brief Returns entry with name "vname" and default value "def"

		//! \fn Entry & getValue(const PIString & vname, const uchar def, bool * exists = 0)
		//! \brief Returns entry with name "vname" and default value "def"

		//! \fn Entry & getValue(const PIString & vname, const ushort def, bool * exists = 0)
		//! \brief Returns entry with name "vname" and default value "def"

		//! \fn Entry & getValue(const PIString & vname, const uint def, bool * exists = 0)
		//! \brief Returns entry with name "vname" and default value "def"

		//! \fn Entry & getValue(const PIString & vname, const ulong def, bool * exists = 0)
		//! \brief Returns entry with name "vname" and default value "def"

		//! \fn Entry & getValue(const PIString & vname, const float def, bool * exists = 0)
		//! \brief Returns entry with name "vname" and default value "def"

		//! \fn Entry & getValue(const PIString & vname, const double def, bool * exists = 0)
		//! \brief Returns entry with name "vname" and default value "def"


		//! Find all entries with names with substrings "vname" and returns them as \a PIConfig::Branch
		Branch getValues(const PIString & vname);


		//! If there is no children returns if name == "name". Else returns if any child has name == "name"
		bool isEntryExists(const PIString & name) const {return entryExists(this, name);}


		//! Convertion to boolean
		operator bool() {return _value.toBool();}

		//! Convertion to char
		operator char() {return (_value.isEmpty() ? 0 : _value[0].toAscii());}

		//! Convertion to short
		operator short() {return _value.toShort();}

		//! Convertion to int
		operator int() {return _value.toInt();}

		//! Convertion to long
		operator long() {return _value.toLong();}

		//! Convertion to uchar
		operator uchar() {return _value.toInt();}

		//! Convertion to ushort
		operator ushort() {return _value.toShort();}

		//! Convertion to uint
		operator uint() {return _value.toInt();}

		//! Convertion to ulong
		operator ulong() {return _value.toLong();}

		//! Convertion to float
		operator float() {return _value.toFloat();}

		//! Convertion to double
		operator double() {return _value.toDouble();}

		//! Convertion to PIString
		operator PIString() {return _value;}

		//! Convertion to PIStringList
		operator PIStringList() {return _value.split("%|%");}

	private:
		typedef PIConfig::Entry * EntryPtr;
		static int compare(const EntryPtr * f, const EntryPtr * s) {return (*f)->_line == (*s)->_line ? 0 : (*f)->_line < (*s)->_line ? -1 : 1;}
		bool entryExists(const Entry * e, const PIString & name) const;
		void buildLine() {_all = _tab + _full_name + " = " + _value + " #" + _type + " " + _comment;}
		void clear() {_children.clear(); _name = _value = _type = _comment = _all = PIString(); _line = 0; _parent = 0;}
		void coutt(std::ostream & s, const PIString & p) const {PIString nl =  p + "  "; if (!_value.isEmpty()) s << p << _name << " = " << _value << std::endl; else std::cout << p << _name << std::endl; piForeachC (Entry * i, _children) i->coutt(s, nl);}
		void piCoutt(PICout s, const PIString & p) const {PIString nl =  p + "  "; if (!_value.isEmpty()) s << p << _name << " = " << _value << PICoutManipulators::NewLine; else std::cout << p << _name << std::endl; piForeachC (Entry * i, _children) i->piCoutt(s, nl);}
		void deleteBranch() {piForeach (Entry * i, _children) {i->deleteBranch(); delete i;}}

		static Entry _empty;
		Entry * _parent;
		mutable Branch _children;
		PIString _tab;
		PIString _name;
		PIString _value;
		PIString _type;
		PIString _comment;
		PIString _all;
		PIString _full_name;
		PIString delim;
		int _line;
	};


	//! Read configuration file at path "path" in mode "mode"
	bool open(const PIString & path, PIIODevice::DeviceMode mode = PIIODevice::ReadWrite);

	//! Read configuration string "string" in mode "mode"
	bool open(PIString * string, PIIODevice::DeviceMode mode = PIIODevice::ReadWrite);\

	bool isOpened() const;

	//! Returns top-level entry with name "vname", if doesn`t exists return entry with value "def" and set *exist to false
	Entry & getValue(const PIString & vname, const PIString & def = PIString(), bool * exists = 0);
	Entry & getValue(const PIString & vname, const PIString & def = PIString(), bool * exists = 0) const {return const_cast<PIConfig * >(this)->getValue(vname, def, exists);}

	PICONFIG_GET_VALUE

	//! \fn Entry & getValue(const PIString & vname, const char * def, bool * exists = 0)
	//! \brief Returns top-level entry with name "vname" and default value "def"

	//! \fn Entry & getValue(const PIString & vname, const char * def, bool * exists = 0)
	//! \brief Returns top-level entry with name "vname" and default value "def"

	//! \fn Entry & getValue(const PIString & vname, const PIStringList & def, bool * exists = 0)
	//! \brief Returns top-level entry with name "vname" and default value "def"

	//! \fn Entry & getValue(const PIString & vname, const bool def, bool * exists = 0)
	//! \brief Returns top-level entry with name "vname" and default value "def"

	//! \fn Entry & getValue(const PIString & vname, const short def, bool * exists = 0)
	//! \brief Returns top-level entry with name "vname" and default value "def"

	//! \fn Entry & getValue(const PIString & vname, const int def, bool * exists = 0)
	//! \brief Returns top-level entry with name "vname" and default value "def"

	//! \fn Entry & getValue(const PIString & vname, const long def, bool * exists = 0)
	//! \brief Returns top-level entry with name "vname" and default value "def"

	//! \fn Entry & getValue(const PIString & vname, const uchar def, bool * exists = 0)
	//! \brief Returns top-level entry with name "vname" and default value "def"

	//! \fn Entry & getValue(const PIString & vname, const ushort def, bool * exists = 0)
	//! \brief Returns top-level entry with name "vname" and default value "def"

	//! \fn Entry & getValue(const PIString & vname, const uint def, bool * exists = 0)
	//! \brief Returns top-level entry with name "vname" and default value "def"

	//! \fn Entry & getValue(const PIString & vname, const ulong def, bool * exists = 0)
	//! \brief Returns top-level entry with name "vname" and default value "def"

	//! \fn Entry & getValue(const PIString & vname, const float def, bool * exists = 0)
	//! \brief Returns top-level entry with name "vname" and default value "def"

	//! \fn Entry & getValue(const PIString & vname, const double def, bool * exists = 0)
	//! \brief Returns top-level entry with name "vname" and default value "def"


	//! Returns top-level entries with names with substrings "vname"
	Branch getValues(const PIString & vname);


	//! Set top-level entry with name "name" value to "value", type to "type" and if "write" immediate write to file. Add new entry if there is no suitable exists
	void setValue(const PIString & name, const PIString & value, const PIString & type = "s", bool write = true);

	//! Set top-level entry with name "name" value to "value", type to "l" and if "write" immediate write to file. Add new entry if there is no suitable exists
	void setValue(const PIString & name, const PIStringList & value, bool write = true) {setValue(name, value.join("%|%"), "l", write);}

	//! Set top-level entry with name "name" value to "value", type to "s" and if "write" immediate write to file. Add new entry if there is no suitable exists
	void setValue(const PIString & name, const char * value, bool write = true) {setValue(name, PIString(value), "s", write);}

	//! Set top-level entry with name "name" value to "value", type to "b" and if "write" immediate write to file. Add new entry if there is no suitable exists
	void setValue(const PIString & name, const bool value, bool write = true) {setValue(name, PIString::fromBool(value), "b", write);}

	//! Set top-level entry with name "name" value to "value", type to "n" and if "write" immediate write to file. Add new entry if there is no suitable exists
	void setValue(const PIString & name, const short value, bool write = true) {setValue(name, PIString::fromNumber(value), "n", write);}

	//! Set top-level entry with name "name" value to "value", type to "n" and if "write" immediate write to file. Add new entry if there is no suitable exists
	void setValue(const PIString & name, const int value, bool write = true) {setValue(name, PIString::fromNumber(value), "n", write);}

	//! Set top-level entry with name "name" value to "value", type to "n" and if "write" immediate write to file. Add new entry if there is no suitable exists
	void setValue(const PIString & name, const long value, bool write = true) {setValue(name, PIString::fromNumber(value), "n", write);}

	//! Set top-level entry with name "name" value to "value", type to "n" and if "write" immediate write to file. Add new entry if there is no suitable exists
	void setValue(const PIString & name, const uchar value, bool write = true) {setValue(name, PIString::fromNumber(value), "n", write);}

	//! Set top-level entry with name "name" value to "value", type to "n" and if "write" immediate write to file. Add new entry if there is no suitable exists
	void setValue(const PIString & name, const ushort value, bool write = true) {setValue(name, PIString::fromNumber(value), "n", write);}

	//! Set top-level entry with name "name" value to "value", type to "n" and if "write" immediate write to file. Add new entry if there is no suitable exists
	void setValue(const PIString & name, const uint value, bool write = true) {setValue(name, PIString::fromNumber(value), "n", write);}

	//! Set top-level entry with name "name" value to "value", type to "n" and if "write" immediate write to file. Add new entry if there is no suitable exists
	void setValue(const PIString & name, const ulong value, bool write = true) {setValue(name, PIString::fromNumber(value), "n", write);}

	//! Set top-level entry with name "name" value to "value", type to "f" and if "write" immediate write to file. Add new entry if there is no suitable exists
	void setValue(const PIString & name, const float value, bool write = true) {setValue(name, PIString::fromNumber(value), "f", write);}

	//! Set top-level entry with name "name" value to "value", type to "f" and if "write" immediate write to file. Add new entry if there is no suitable exists
	void setValue(const PIString & name, const double value, bool write = true) {setValue(name, PIString::fromNumber(value), "f", write);}

	//! Returns root entry
	Entry & rootEntry() {return root;}

	//! Returns top-level entries count
	int entriesCount() const {return childCount(&root);}

	//! Returns if top-level entry with name "name" exists
	bool isEntryExists(const PIString & name) const {return entryExists(&root, name);}

	//! Returns all top-level entries
	Branch allTree() {Branch b; piForeach (Entry * i, root._children) b << i; b.delim = delim; return b;}

	//! Returns all entries without children
	Branch allLeaves() {Branch b; allLeaves(b, &root); b.sort(Entry::compare); b.delim = delim; return b;}

	int entryIndex(const PIString & name);

	PIString getName(uint number) {return entryByIndex(number)._name;}
	PIString getValue(uint number) {return entryByIndex(number)._value;}
	PIChar getType(uint number) {return entryByIndex(number)._type[0];}
	PIString getComment(uint number) {return entryByIndex(number)._comment;}

	void addEntry(const PIString & name, const PIString & value, const PIString & type = "s", bool write = true);
	void setName(uint number, const PIString & name, bool write = true);
	void setValue(uint number, const PIString & value, bool write = true);
	void setType(uint number, const PIString & type, bool write = true);
	void setComment(uint number, const PIString & comment, bool write = true);

	void removeEntry(const PIString & name, bool write = true);
	void removeEntry(uint number, bool write = true);

	//! Remove all tree and device content
	void clear();

	//! Parse device and build internal tree
	void readAll();

	//! Write all internal tree to device
	void writeAll();

	//! Returns current tree delimiter, default "."
	const PIString & delimiter() const {return delim;}

	//! Set current tree delimiter
	void setDelimiter(const PIString & d) {delim = d; setEntryDelim(&root, d); readAll();}

private:
	PIConfig(const PIString & path, PIStringList dirs);
	void _init();
	void _clearDev();
	void _flushDev();
	bool _isEndDev();
	void _seekToBeginDev();
	PIString _readLineDev();
	void _writeDev(const PIString & l);
	int childCount(const Entry * e) const {int c = 0; piForeachC (Entry * i, e->_children) c += childCount(i); c += e->_children.size_s(); return c;}
	bool entryExists(const Entry * e, const PIString & name) const;
	void buildFullNames(Entry * e) {piForeach (Entry * i, e->_children) {if (e != &root) i->_full_name = e->_full_name + delim + i->_name; else i->_full_name = i->_name; buildFullNames(i);}}
	void allLeaves(Branch & b, Entry * e) {piForeach (Entry * i, e->_children) {if ((!i->_value.isEmpty() && !i->isLeaf()) || i->isLeaf()) b << i; allLeaves(b, i);}}
	void setEntryDelim(Entry * e, const PIString & d) {piForeach (Entry * i, e->_children) setEntryDelim(i, d); e->delim = d;}
	Entry & entryByIndex(const int index) {Branch b = allLeaves(); if (index < 0 || index >= b.size_s()) return empty; return *(b[index]);}
	void removeEntry(Branch & b, Entry * e);
	void deleteEntry(Entry * e) {piForeach (Entry * i, e->_children) deleteEntry(i); delete e;}
	PIString getPrefixFromLine(PIString line, bool * exists);
	void updateIncludes();
	PIString parseLine(PIString v);
	void parse();

	bool own_dev, internal;
	PIVector<PIConfig * > includes, inc_devs;
	Branch all_includes;
	PIIODevice * dev;
	PIString delim;
	PIStringList incdirs;
	Entry root, empty;
	uint lines;
	PIStringList other;

};


inline std::ostream & operator <<(std::ostream & s, const PIConfig::Branch & v) {v.coutt(s, ""); return s;}
inline std::ostream & operator <<(std::ostream & s, const PIConfig::Entry & v) {s << v.value(); return s;}
inline PICout operator <<(PICout s, const PIConfig::Branch & v) {s.setControl(0, true); v.piCoutt(s, ""); s.restoreControl(); return s;}
inline PICout operator <<(PICout s, const PIConfig::Entry & v) {s << v.value(); return s;}


/** \relatesalso PIConfig \relatesalso PIIODevice
 * \brief Service function. useful for configuring devices
 * \details Function takes entry name "name", default value "def" and two
 * \a PIConfig::Entry sections: "em" and their parent "ep". If there is no
 * parent ep = 0. If "ep" is not null and entry "name" exists in "ep" function
 * returns this value. Else returns value of entry "name" in section "em" or
 * "def" if entry doesn`t exists. \n This function useful to read settings
 * from configuration file in implementation \a PIIODevice::configureDevice() function */
template<typename T>
T readDeviceSetting(const PIString & name, const T & def, const PIConfig::Entry * em, const PIConfig::Entry * ep) {
	if (ep != 0) {
		T ret;
		bool ex;
		ret = ep->getValue(name, def, &ex);
		if (!ex) ret = em->getValue(name, def);
		return ret;
	}
	return em->getValue(name, def);

}

#endif // PICONFIG_H
