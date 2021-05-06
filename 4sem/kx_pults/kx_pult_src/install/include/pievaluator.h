/*! \file pievaluator.h
 * \brief Mathematic expressions calculator
*/
/*
	PIP - Platform Independent Primitives
	Evaluator designed for stream calculations
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

#ifndef PIEVALUATOR_H
#define PIEVALUATOR_H

#include "pistring.h"
#include "pimathbase.h"

typedef complexd (*FuncFunc)(void * , int, complexd * );

namespace PIEvaluatorTypes {
	static const int operationCount = 14;

	enum eType {etNumber, etOperator, etVariable, etFunction};
	enum Operation {oNone, oAdd, oSubtract, oMultiply, oDivide, oResidue, oPower,
					oEqual, oNotEqual, oGreater, oSmaller, oGreaterEqual, oSmallerEqual,
					oAnd, oOr, oFunction
	};
	enum BaseFunctions {bfUnknown, bfSin, bfCos, bfTg, bfCtg,
						bfArcsin, bfArccos, bfArctg, bfArcctg,
						bfExp, bfRandom, bfSh, bfCh, bfTh, bfCth,
						bfSqrt, bfSqr, bfPow, bfAbs,
						bfLn, bfLg, bfLog, bfSign,
						bfIm, bfRe, bfArg, bfLen, bfConj,
						bfRad, bfDeg, bfJ0, bfJ1, bfJN,
						bfY0, bfY1, bfYN, bfMin, bfMax,
						bfClamp, bfStep, bfMix, bfDefined,
						bfCustom = 0xFFFF
	};

	struct Instruction {
		Instruction() {out = -1; function = -1; operation = oNone;}
		Instruction(Operation oper, PIVector<int> opers, int out_ind, int func = -1) {
			operation = oper; operators = opers; out = out_ind; function = func;}
		Operation operation;
		PIVector<int> operators;
		int out;
		int function;
	};
	struct Element {
		Element() {num = 0; var_num = -1; type = etNumber;}
		Element(eType new_type, int new_num, int new_var_num = -1) {set(new_type, new_num, new_var_num);}
		void set(eType new_type, int new_num, int new_var_num = -1) {type = new_type; num = new_num; var_num = new_var_num;}
		eType type;
		int num;
		int var_num;
	};
	struct Function {
		Function() {arguments = 0; type = bfUnknown; handler = 0;}
		Function(const PIString & name, int args, BaseFunctions ftype) {identifier = name; arguments = args; type = ftype; handler = 0;}
		Function(const PIString & name, int args, FuncFunc h) {identifier = name; arguments = args; type = bfCustom; handler = h;}
		PIString identifier;
		BaseFunctions type;
		FuncFunc handler;
		int arguments;
	};
	struct Variable {
		Variable() {value = 0.;}
		Variable(const PIString & var_name, complexd val) {name = var_name; value = val;}
		PIString name;
		complexd value;
	};
};
/*
	≠	:
	≥	}
	≤	{
	⋀	&
	⋁	|
*/

class PIP_EXPORT PIEvaluatorContent
{
	friend class PIEvaluator;
public:
	PIEvaluatorContent();
	~PIEvaluatorContent() {;}

	void addFunction(const PIString & name, int args = 1) {functions.push_back(PIEvaluatorTypes::Function(name, args, getBaseFunction(name)));}
	void addVariable(const PIString & name, const complexd & val = 0.) {variables.push_back(PIEvaluatorTypes::Variable(name, val)); sortVariables();}
	void addCustomFunction(const PIString & name, int args_count, FuncFunc func) {functions << PIEvaluatorTypes::Function(name, args_count, func);}
	int functionsCount() const {return functions.size();}
	int variablesCount() const {return variables.size();}
	int customVariablesCount() const {return variables.size() - cv_count;}
	int findFunction(const PIString & name) const {for (uint i = 0; i < functions.size(); i++) if (functions[i].identifier == name) return i; return -1;}
	int findVariable(const PIString & var_name) const {for (uint i = 0; i < variables.size(); i++) if (variables[i].name == var_name) return i; return -1;}
	PIEvaluatorTypes::Function function(int index) {if (index < 0 || index >= functions.size_s()) return PIEvaluatorTypes::Function(); return functions[index];}
	PIEvaluatorTypes::Variable variable(int index) {if (index < 0 || index >= variables.size_s()) return PIEvaluatorTypes::Variable(); return variables[index];}
	PIEvaluatorTypes::Function function(const PIString & name) {return function(findFunction(name));}
	PIEvaluatorTypes::Variable variable(const PIString & name) {return variable(findVariable(name));}
	PIEvaluatorTypes::Variable customVariable(int index) {if (index < cv_count || index >= variables.size_s() + cv_count) return PIEvaluatorTypes::Variable(); return variables[index + cv_count];}
	bool setVariableValue(int index, complexd new_value);
	bool setVariableName(int index, const PIString & new_name);
	bool setVariableValue(const PIString & var_name, const complexd & new_value) {return setVariableValue(findVariable(var_name), new_value);}
	bool setVariableName(const PIString & var_name, const PIString & new_name) {return setVariableName(findVariable(var_name), new_name);}
	void removeVariable(int index) {variables.remove(index);}
	void removeVariable(const PIString & var_name) {removeVariable(findVariable(var_name));}
	void clearCustomVariables();
	void sortVariables();
	PIEvaluatorTypes::BaseFunctions getBaseFunction(const PIString & name);

private:
	PIVector<PIEvaluatorTypes::Function> functions;
	PIVector<PIEvaluatorTypes::Variable> variables;
	int cv_count;

};


class PIP_EXPORT PIEvaluator
{
public:

	//! Constructs an empty evaluator
	PIEvaluator() {correct = false; data_ = 0;}

	~PIEvaluator() {;}


	//! Returns custom data
	void * data() {return data_;}

	//! Set custom data to "_data"
	void setData(void * _data) {data_ = _data;}


	//! Check mathematical expression and parse it to list of instructions
	bool check(const PIString & string);

	//! Returns true if expression was checked succesfully
	bool isCorrect() const {return correct;}

	//! Set variable value with name "name" to value "value". Add variable if it doesn`t exists
	int setVariable(const PIString & name, complexd value = 0.) {if (content.findVariable(name) < 0) content.addVariable(name, value); else content.setVariableValue(name, value); return content.findVariable(name);}

	//! Set variable value with index "index" to value "value". Don`t add variable if it doesn`t exists
	void setVariable(int index, complexd value = 0.) {if (index >= 0 && index < content.variablesCount()) content.setVariableValue(index, value);}

	void setCustomVariableValue(int index, complexd value = 0.) {content.variables[index + content.cv_count].value = value;}
/*
	//! Add function "name" with arguments count "args_count" and handler "func". Three arguments will be passed to handler: \a data(), "args_count" and array of input values.
	void addFunction(const PIString & name, int args_count, FuncFunc func) {content.addCustomFunction(name, args_count, func);}
*/
	//! Evaluate last successfully checked with function \a check() expression and returns result
	complexd evaluate();

	//! Remove variable with name "name"
	void removeVariable(const PIString & name) {content.removeVariable(name);}

	//! Remove all manually added variables
	void clearCustomVariables() {content.clearCustomVariables();}

	//! Returns index of variable with name "name"
	int variableIndex(const PIString & name) const {return content.findVariable(name);}

	//! Returns all unknown variables founded in last expression passed to \a check() function
	const PIStringList & unknownVariables() const {return unknownVars;}

	//! Returns processed last expression passed to \a check() function
	const PIString & expression() const {return currentString;}

	//! Returns last error description occured in \a check() function
	const PIString & error() const {return lastError;}

	//! Returns last result of \a evaluate()
	const complexd & lastResult() const {return out;}

	PIEvaluatorContent content;

private:
	const PIString & prepare(const PIString & string);
	const PIString & preprocess(const PIString & string);
	int parse(const PIString & string, int offset = 0);
	void convert();
	void checkBrackets();
	void removeSpaces();
	void findUnknownVariables();
	void removeJunk();
	void replaceOperators();
	void makeOutput(PIString & string);
	bool fillElements();
	bool setSignes();
	bool isSign(const PIChar & ch);
	PIString inverse(const PIString & string) {int len = string.length(); PIString s; for (int i = 0; i < len; i++) s += string[len - i - 1]; return s;}
	bool check();
	bool execInstructions();
	PIString inBrackets(const PIString & string);
	PIString operationChar(const PIEvaluatorTypes::Operation & operation);
	PIEvaluatorTypes::Operation operationInOrder(const int & index);
	complexd value(const int & index) {if (index < 0) return tmpvars[-index - 1].value; else return kvars->at(index).value;}
	inline complexd residue(const complexd & f, const complexd & s);
	inline void execFunction(const PIEvaluatorTypes::Instruction & ci);

	PIVector<PIEvaluatorTypes::Element> elements;
	PIVector<PIEvaluatorTypes::Variable> currentVariables, variables, tmpvars, * kvars;
	PIVector<PIEvaluatorTypes::Instruction> instructions;
	PIStringList unknownVars;
	PIString currentString, lastError;
	complexd out;
	bool correct;
	void * data_;
};

inline bool operator ==(PIEvaluatorTypes::Element e1, PIEvaluatorTypes::Element e2) {return (e1.type == e2.type && e1.num == e2.num);}
inline bool operator !=(PIEvaluatorTypes::Element e1, PIEvaluatorTypes::Element e2) {return (e1.type != e2.type || e1.num != e2.num);}

#endif // PIEVALUATOR_H
