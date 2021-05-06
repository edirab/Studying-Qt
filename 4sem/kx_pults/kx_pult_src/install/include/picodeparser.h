/*! \file picodeparser.h
 * \brief C++ code parser
*/
/*
	PIP - Platform Independent Primitives
	C++ code parser
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


#ifndef PICODEPARSER_H
#define PICODEPARSER_H

#include "pifile.h"
#include "pievaluator.h"

inline bool _isCChar(const PIChar & c) {return (c.isAlpha() || (c.toAscii() == '_'));}
inline bool _isCChar(const PIString & c) {if (c.isEmpty()) return false; return _isCChar(c[0]);}

class PIP_EXPORT PICodeParser {
public:
	PICodeParser();
	
	enum PIP_EXPORT Visibility {Global, Public, Protected, Private};
	enum PIP_EXPORT Attribute {
		NoAttributes = 0x0,
		Const = 0x01,
		Static = 0x02,
		Mutable = 0x04,
		Volatile = 0x08,
		Inline = 0x10,
		Virtual = 0x20,
		Extern = 0x40
	};
	
	typedef PIFlags<Attribute> Attributes;
	typedef PIPair<PIString, PIString> Define;
	typedef PIPair<PIString, PIString> Typedef;
	typedef PIPair<PIString, int> Enumerator;
	
	struct PIP_EXPORT Macro {
		Macro(const PIString & n = PIString(), const PIString & v = PIString(), const PIStringList & a = PIStringList()) {
			name = n;
			value = v;
			args = a;
		}
		PIString expand(PIString args_, bool * ok = 0) const;
		PIString name;
		PIString value;
		PIStringList args;
	};
	
	struct PIP_EXPORT Member {
		Member() {
			visibility = Global;
			size = 0;
			is_type_ptr = false;
			attributes = NoAttributes;
		}
		PIString type;
		PIString name;
		PIStringList arguments_full;
		PIStringList arguments_type;
		Visibility visibility;
		Attributes attributes;
		bool is_type_ptr;
		int size;
	};
	
	struct PIP_EXPORT Entity {
		Entity() {
			visibility = Global;
			size = 0;
		}
		PIString type;
		PIString name;
		PIString file;
		Visibility visibility;
		int size;
		PIVector<Entity * > parents;
		//PIVector<Entity * > children;
		PIVector<Member> functions;
		PIVector<Member> members;
		PIVector<Typedef> typedefs;
	};
	
	struct PIP_EXPORT Enum {
		Enum(const PIString & n = PIString()) {
			name = n;
		}
		PIString name;
		PIVector<Enumerator> members;
	};
	
	void parseFile(const PIString & file, bool follow_includes = true);
	void parseFiles(const PIStringList & files, bool follow_includes = true);
	
	void includeDirectory(const PIString & dir) {includes << dir;}
	void addDefine(const PIString & def_name, const PIString & def_value) {custom_defines << Define(def_name, def_value);}
	bool isEnum(const PIString & name);
	Entity * findEntityByName(const PIString & en);
	PIStringList parsedFiles() const {return PIStringList(proc_files.toVector());}
	PIString mainFile() const {return main_file;}
	const PICodeParser::Entity * global() const {return &root_;}
	
	int macrosSubstitutionMaxIterations() const {return macros_iter;}
	void setMacrosSubstitutionMaxIterations(int value) {macros_iter = value;}
	
	PIVector<Define> defines, custom_defines;
	PIVector<Macro> macros;
	PIVector<Enum> enums;
	PIVector<Typedef> typedefs;
	PIVector<Entity * > entities;
	
private:
	void clear();
	bool parseFileInternal(const PIString & file, bool follow_includes);
	bool parseFileContent(PIString & fc, bool main);
	bool parseDirective(PIString d);
	Entity * parseClassDeclaration(const PIString & fc);
	PIString parseClass(PIString & fc);
	bool parseEnum(const PIString & name, PIString fc);
	Typedef parseTypedef(PIString fc);
	bool parseMember(Entity * parent, PIString & fc);
	void restoreTmpTemp(Member * e);
	bool macroCondition(const PIString & mif, PIString mifcond);
	bool isDefineExists(const PIString & dn);
	double defineValue(const PIString & dn);
	PIString procMacros(PIString fc);
	double procMacrosCond(PIString fc);
	bool isDeclaration(const PIString & fc, int start, int * end);
	bool isMainFile(const PIString & fc);
	void normalizeEntityNamespace(PIString & n);
	
	int macros_iter;
	bool with_includes;
	PIEvaluator evaluator;
	//PIVector<Entity * > tree;
	PISet<PIString> proc_files;
	PIString cur_file, main_file;
	PIStringList includes;
	Entity root_;
	Visibility cur_def_vis;
	PIString cur_namespace;
	PIMap<PIString, PIString> tmp_temp;
	
};

#endif // PICODEPARSER_H
