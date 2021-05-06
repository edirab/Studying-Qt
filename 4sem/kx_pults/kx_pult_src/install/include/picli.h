/*! \file picli.h
 * \brief Command-Line parser
*/
/*
    PIP - Platform Independent Primitives
    Command-Line Parser
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

#ifndef PICLI_H
#define PICLI_H

#include "piobject.h"

class PIP_EXPORT PICLI: public PIObject
{
	PIOBJECT_SUBCLASS(PICLI, PIObject)
public:
	
	//! Constructor
	PICLI(int argc, char * argv[]);
	
	
	//! Add argument with name "name", short key = name first letter, full key = name
	void addArgument(const PIString & name, bool value = false) {_args << Argument(name, name[0], name, value); needParse = true;}
	
	//! Add argument with name "name", short key = "shortKey", full key = name
	void addArgument(const PIString & name, const PIChar & shortKey, bool value = false) {_args << Argument(name, shortKey, name, value); needParse = true;}
	
	//! Add argument with name "name", short key = "shortKey", full key = name
	void addArgument(const PIString & name, const char * shortKey, bool value = false) {_args << Argument(name, PIChar(shortKey), name, value); needParse = true;}
	
	//! Add argument with name "name", short key = "shortKey", full key = "fullKey"
	void addArgument(const PIString & name, const PIChar & shortKey, const PIString & fullKey, bool value = false) {_args << Argument(name, shortKey, fullKey, value); needParse = true;}
	
	//! Add argument with name "name", short key = "shortKey", full key = "fullKey"
	void addArgument(const PIString & name, const char * shortKey, const PIString & fullKey, bool value = false) {_args << Argument(name, PIChar(shortKey), fullKey, value); needParse = true;}
	
	
	//! Returns unparsed command-line argument by index "index". Index 0 is program execute command.
	PIString rawArgument(int index) {parse(); return _args_raw[index];}
	PIString mandatoryArgument(int index) {parse(); return _args_mand[index];}
	PIString optionalArgument(int index) {parse(); return _args_opt[index];}
	
	//! Returns unparsed command-line arguments
	const PIStringList & rawArguments() {parse(); return _args_raw;}
	const PIStringList & mandatoryArguments() {parse(); return _args_mand;}
	const PIStringList & optionalArguments() {parse(); return _args_opt;}
	
	//! Returns program execute command without arguments
	PIString programCommand() {parse(); return _args_raw.size() > 0 ? _args_raw.front() : PIString();}
	bool hasArgument(const PIString & name) {parse(); piForeach (Argument & i, _args) if (i.name == name && i.found) return true; return false;}
	PIString argumentValue(const PIString & name) {parse(); piForeach (Argument &i, _args) if (i.name == name && i.found) return i.value; return PIString();}
	PIString argumentShortKey(const PIString & name) {piForeach (Argument &i, _args) if (i.name == name) return i.short_key; return PIString();}
	PIString argumentFullKey(const PIString & name) {piForeach (Argument &i, _args) if (i.name == name) return i.full_key; return PIString();}
	
	const PIString & shortKeyPrefix() const {return _prefix_short;}
	const PIString & fullKeyPrefix() const {return _prefix_full;}
	int mandatoryArgumentsCount() const {return _count_mand;}
	int optionalArgumentsCount() const {return _count_opt;}
	void setShortKeyPrefix(const PIString & prefix) {_prefix_short = prefix; needParse = true;}
	void setFullKeyPrefix(const PIString & prefix) {_prefix_full = prefix; needParse = true;}
	void setMandatoryArgumentsCount(const int count) {_count_mand = count; needParse = true;}
	void setOptionalArgumentsCount(const int count) {_count_opt = count; needParse = true;}
	
private:
	struct Argument {
		Argument() {has_value = found = false;}
		Argument(const PIString & n, const PIChar & s, const PIString & f, bool v) {name = n; short_key = s; full_key = f; has_value = v; found = false;}
		PIString name;
		PIChar short_key;
		PIString full_key;
		PIString value;
		bool has_value, found;
	};
	
	void parse();
	
	PIString _prefix_short, _prefix_full;
	PIStringList _args_raw, _args_mand, _args_opt;
	PISet<PIString> keys_full, keys_short;
	PIVector<Argument> _args;
	int _count_mand, _count_opt;
	bool needParse;
	
};

#endif // PICLI_H
