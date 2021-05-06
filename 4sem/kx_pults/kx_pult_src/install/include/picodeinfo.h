/*! \file picodeinfo.h
 * \brief C++ code info structs
*/
/*
    PIP - Platform Independent Primitives
    C++ code info structs
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


#ifndef PICODEINFO_H
#define PICODEINFO_H

#include "pistring.h"

namespace PICodeInfo {

enum TypeFlag {
	NoFlag,
	Const = 0x01,
	Static = 0x02,
	Mutable = 0x04,
	Volatile = 0x08,
	Inline = 0x10,
	Virtual = 0x20,
	Extern = 0x40
};

typedef PIFlags<PICodeInfo::TypeFlag> TypeFlags;

struct TypeInfo {
	TypeInfo(const PIString & n = PIString(), const PIString & t = PIString(), PICodeInfo::TypeFlags f = 0) {name = n; type = t; flags = f;}
	PIString name;
	PIString type;
	PICodeInfo::TypeFlags flags;
};

struct FunctionInfo {
	PIString name;
	TypeInfo return_type;
	PIVector<PICodeInfo::TypeInfo> arguments;
};

struct ClassInfo {
	PIString name;
	PIStringList parents;
	PIVector<PICodeInfo::TypeInfo> variables;
	PIVector<PICodeInfo::FunctionInfo> functions;
};

struct EnumeratorInfo {
	EnumeratorInfo(const PIString & n = PIString(), int v = 0) {name = n; value = v;}
	PIString name;
	int value;
};

struct EnumInfo {
	PIString memberName(int value) const;
	int memberValue(const PIString & name) const;
	PIString name;
	PIVector<PICodeInfo::EnumeratorInfo> members;
};


inline PICout operator <<(PICout s, const PICodeInfo::TypeInfo & v) {
	if (v.flags[Inline]) s << "inline ";
	if (v.flags[Virtual]) s << "virtual ";
	if (v.flags[Mutable]) s << "mutable ";
	if (v.flags[Volatile]) s << "volatile ";
	if (v.flags[Static]) s << "static ";
	if (v.flags[Const]) s << "const ";
	s << v.type;
	if (!v.name.isEmpty())
		s << " " << v.name;
	return s;
}

inline PICout operator <<(PICout s, const PICodeInfo::EnumeratorInfo & v) {s << v.name << " = " << v.value; return s;}

inline PICout operator <<(PICout s, const PICodeInfo::ClassInfo & v) {
	s.setControl(0, true);
	s << "class " << v.name;
	if (!v.parents.isEmpty()) {
		s << ": ";
		bool first = true;
		piForeachC (PIString & i, v.parents) {
			if (first) first = false;
			else s << ", ";
			s << i;
		}
	}
	s << " {\n";
	piForeachC (FunctionInfo & i, v.functions) {
		s << PICoutManipulators::Tab << i.return_type << " " << i.name << "(";
		bool fa = true;
		piForeachC (TypeInfo & a, i.arguments) {
			if (fa) fa = false;
			else s << ", ";
			s << a;
		}
		s << ");\n";
	}
	if (!v.functions.isEmpty() && !v.variables.isEmpty())
		s << "\n";
	piForeachC (TypeInfo & i, v.variables) {
		s << PICoutManipulators::Tab << i << ";\n";
	}
	s << "}\n";
	s.restoreControl();
	return s;
}

inline PICout operator <<(PICout s, const PICodeInfo::EnumInfo & v) {
	s.setControl(0, true);
	s << "enum " << v.name << " {\n";
	piForeachC (EnumeratorInfo & i, v.members) {
		bool f = true;
		if (f) f = false;
		else s << ", ";
		s << PICoutManipulators::Tab << i << "\n";
	}
	s << "}\n";
	s.restoreControl();
	return s;
}

extern PIMap<PIString, PICodeInfo::ClassInfo * > * classesInfo;
extern PIMap<PIString, PICodeInfo::EnumInfo * > * enumsInfo;

}

class __PICodeInfoInitializer__ {
public:
	__PICodeInfoInitializer__() {
		if (_inited_) return;
		_inited_ = true;
		PICodeInfo::classesInfo = new PIMap<PIString, PICodeInfo::ClassInfo * >;
		PICodeInfo::enumsInfo = new PIMap<PIString, PICodeInfo::EnumInfo * >;
	}
	static bool _inited_;
};

static __PICodeInfoInitializer__ __picodeinfoinitializer__;

#endif // PICODEINFO_H
