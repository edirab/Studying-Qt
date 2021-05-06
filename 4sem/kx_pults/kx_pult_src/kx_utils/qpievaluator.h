/*
    Peri4 Paint
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

#ifndef QPIEVALUATOR_H
#define QPIEVALUATOR_H

#include <QStringList>
#include <QVector>
#include <QMap>
#include <QDebug>
#include <cmath>
#include <complex>

#ifndef PIP_VERSION
typedef std::complex<double> complexd;
const complexd complexd_i(0., 1.);
const complexd complexd_0(0.);
const complexd complexd_1(1.);
#endif
const double deg2rad_qpie = atan(1.) / 45.;
const double rad2deg_qpie = 45. / atan(1.);

inline complexd round_qpie(const complexd & c) {return complexd(round(c.real()), round(c.imag()));}
inline complexd floor_qpie(const complexd & c) {return complexd(floor(c.real()), floor(c.imag()));}
inline complexd  ceil_qpie(const complexd & c) {return complexd(ceil(c.real()), ceil(c.imag()));}
inline complexd atanc_qpie(const complexd & c) {return -complexd(-0.5, 1.) * log((complexd_1 + complexd_i * c) / (complexd_1 - complexd_i * c));}
inline complexd asinc_qpie(const complexd & c) {return -complexd_i * log(complexd_i * c + sqrt(complexd_1 - c * c));}
inline complexd acosc_qpie(const complexd & c) {return -complexd_i * log(c + complexd_i * sqrt(complexd_1 - c * c));}

namespace QPIEvaluatorTypes {
	static const int operationCount = 14;

	enum eType {etNumber, etOperator, etVariable, etFunction};
	enum Operation {oNone, oAdd, oSubtract, oMultiply, oDivide, oResidue, oPower,
					oEqual, oNotEqual, oGreater, oSmaller, oGreaterEqual, oSmallerEqual,
					oAnd, oOr, oFunction};
	enum BaseFunctions {bfUnknown, bfSin, bfCos, bfTg, bfCtg,
						bfArcsin, bfArccos, bfArctg, bfArcctg,
						bfExp, bfRandom, bfSh, bfCh, bfTh, bfCth,
						bfSqrt, bfSqr, bfPow, bfAbs,
						bfLn, bfLg, bfLog, bfSign,
						bfIm, bfRe, bfArg, bfLen, bfConj,
						bfRad, bfDeg};

	struct Instruction {
		Instruction() {;}
		Instruction(Operation oper, QVector<int> opers, int out_ind, int func = -1) {
			operation = oper; operators = opers; out = out_ind; function = func;}
		Operation operation;
		QVector<int> operators;
		int out;
		int function;};
	struct Element {
		Element() {;}
		Element(eType new_type, int new_num, int new_var_num = -1) {set(new_type, new_num, new_var_num);}
		void set(eType new_type, int new_num, int new_var_num = -1) {type = new_type; num = new_num; var_num = new_var_num;}
		eType type;
		int num;
		int var_num;};
	struct Function {
		Function() {arguments = 0; type = bfUnknown;}
		Function(const QString & name, int args, BaseFunctions ftype) {identifier = name; arguments = args; type = ftype;}
		QString identifier;
		BaseFunctions type;
		int arguments;};
	struct Variable {
		Variable() {value = 0.;}
		Variable(const QString & var_name, complexd val) {name = var_name; value = val;}
		QString name;
		complexd value;};
	inline bool operator <(const Variable & s1, const Variable & s2) {
		if (s1.name.size() != s2.name.size())
			return s1.name.size() > s2.name.size();
		return s1.name > s2.name;
	}
};
/*
	≠	:
	≥	}
	≤	{
	⋀	&
	⋁	|
*/
class QPIEvaluatorContent
{
	friend class QPIEvaluator;
public:
	QPIEvaluatorContent();
	~QPIEvaluatorContent() {;}

	void addFunction(const QString & name, int args = 1) {functions.push_back(QPIEvaluatorTypes::Function(name, args, getBaseFunction(name)));}
	void addVariable(const QString & name, const complexd & val = 0., bool sort = true) {variables.push_back(QPIEvaluatorTypes::Variable(name, val)); if (sort) sortVariables();}
	int functionsCount() const {return functions.size();}
	int variablesCount() const {return variables.size();}
	int customVariablesCount() const {return variables.size() - cv_count;}
	int findFunction(const QString & name) const {return func_index.value(name, -1);}
	int findVariable(const QString & name) const {return var_index.value(name, -1);}
	QPIEvaluatorTypes::Function function(int index) {if (index < 0 || index >= functions.size()) return QPIEvaluatorTypes::Function(); return functions[index];}
	QPIEvaluatorTypes::Variable variable(int index) {if (index < 0 || index >= variables.size()) return QPIEvaluatorTypes::Variable(); return variables[index];}
	QPIEvaluatorTypes::Function function(const QString & name) {return function(findFunction(name));}
	QPIEvaluatorTypes::Variable variable(const QString & name) {return variable(findVariable(name));}
	QPIEvaluatorTypes::Variable customVariable(int index) {if (index < cv_count || index >= variables.size() + cv_count) return QPIEvaluatorTypes::Variable(); return variables[index + cv_count];}
	bool setVariableValue(int index, complexd new_value);
	bool setVariableName(int index, const QString & new_name);
	bool setVariableValue(const QString & var_name, const complexd & new_value) {return setVariableValue(findVariable(var_name), new_value);}
	bool setVariableName(const QString & var_name, const QString & new_name) {return setVariableName(findVariable(var_name), new_name);}
	void removeVariable(int index) {variables.remove(index);}
	void removeVariable(const QString & var_name) {removeVariable(findVariable(var_name));}
	void clearCustomVariables();
	void sortVariables();
	QPIEvaluatorTypes::BaseFunctions getBaseFunction(const QString & name);

private:
	QVector<QPIEvaluatorTypes::Function> functions;
	QVector<QPIEvaluatorTypes::Variable> variables;
	QMap<QString, int> var_index, func_index;
	int cv_count;

};

class QPIEvaluator
{
public:
	QPIEvaluator() {correct = false ;}
	~QPIEvaluator() {;}

	bool check(const QString & string);
	bool isCorrect() const {return correct;}
	int setVariable(const QString & name, complexd value = 0.);
	void setVariable(int index, complexd value = 0.) {if (index >= 0 && index < content.variablesCount()) content.setVariableValue(index, value);}
	void setCustomVariableValue(int index, complexd value = 0.) {content.variables[index + content.cv_count].value = value;}
	complexd evaluate();
	void removeVariable(const QString & name) {content.removeVariable(name);}
	void clearCustomVariables() {content.clearCustomVariables();}
	int variableIndex(const QString & name) const {return content.findVariable(name);}
	const QStringList & unknownVariables() const {return unknownVars;}
	const QString & expression() const {return currentString;}
	const QString & error() const {return lastError;}
	const complexd & lastResult() const {return out;}
	static QString inBrackets(const QString & string);

	QPIEvaluatorContent content;

private:
	const QString & prepare(const QString & string);
	const QString & preprocess(const QString & string);
	int parse(const QString & string, int offset = 0);
	void convert();
	void checkBrackets();
	void removeSpaces();
	void findUnknownVariables();
	void removeJunk();
	void replaceOperators();
	void makeOutput(QString & string);
	bool fillElements();
	bool setSignes();
	bool isSign(const QChar & ch);
	QString inverse(const QString & string) {int len = string.length(); QString s; for (int i = 0; i < len; i++) s += string[len - i - 1]; return s;}
	bool check();
	bool execInstructions();
	QString operationChar(const QPIEvaluatorTypes::Operation & operation);
	QPIEvaluatorTypes::Operation operationInOrder(const int & index);
	complexd value(const int & index) {if (index < 0) return tmpvars[-index - 1].value; else return kvars->at(index).value;}
	inline complexd residue(const complexd & f, const complexd & s);
	inline void execFunction(const QPIEvaluatorTypes::Instruction & ci);

	QVector<QPIEvaluatorTypes::Element> elements;
	QVector<QPIEvaluatorTypes::Variable> currentVariables, variables, tmpvars, * kvars;
	QVector<QPIEvaluatorTypes::Instruction> instructions;
	QStringList unknownVars;
	QString currentString, lastError;
	complexd out;
	bool correct;

};

inline bool operator ==(QPIEvaluatorTypes::Element e1, QPIEvaluatorTypes::Element e2) {return (e1.type == e2.type && e1.num == e2.num);}
inline bool operator !=(QPIEvaluatorTypes::Element e1, QPIEvaluatorTypes::Element e2) {return (e1.type != e2.type || e1.num != e2.num);}

#endif // QPIEVALUATOR_H
