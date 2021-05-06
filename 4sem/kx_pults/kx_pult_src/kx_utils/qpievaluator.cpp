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

#include "qpievaluator.h"


QPIEvaluatorContent::QPIEvaluatorContent() {
	addFunction("arcsin", 1);
	addFunction("arccos", 1);
	addFunction("arctg", 1);
	addFunction("arcctg", 1);
	addFunction("random", 2);
	addFunction("sin", 1);
	addFunction("cos", 1);
	addFunction("ctg", 1);
	addFunction("tg", 1);
	addFunction("exp", 1);
	addFunction("cth", 1);
	addFunction("sh", 1);
	addFunction("ch", 1);
	addFunction("th", 1);
	addFunction("sqrt", 1);
	addFunction("sqr", 1);
	addFunction("pow", 2);
	addFunction("abs", 1);
	addFunction("ln", 1);
	addFunction("lg", 1);
	addFunction("log", 2);
	addFunction("im", 1);
	addFunction("re", 1);
	addFunction("arg", 1);
	addFunction("len", 1);
	addFunction("conj", 1);
	addFunction("sign", 1);
	addFunction("rad", 1);
	addFunction("deg", 1);
	clearCustomVariables();
	//addVariable("n", 0.);
	//addVariable("x1", 123);
}


bool QPIEvaluatorContent::setVariableValue(int index, complexd new_value) {
	if (index < 0 || index >= variables.size()) return false;
	variables[index].value = new_value;
	return true;
}


bool QPIEvaluatorContent::setVariableName(int index, const QString & new_name) {
	if (index < 0 || index >= variables.size()) return false;
	variables[index].name = new_name;
	return true;
}


void QPIEvaluatorContent::clearCustomVariables() {
	var_index.clear();
	func_index.clear();
	variables.clear();
	addVariable("i", complexd_i);
	addVariable("pi", atan(1.) * 4.);
	addVariable("e", exp(1.));
	cv_count = variables.size();
}


void QPIEvaluatorContent::sortVariables() {
	var_index.clear();
	qSort(variables.begin(), variables.end());
	for (int i = 0; i < variables.size(); i++)
		var_index[variables[i].name] = i;
	/*
	qDebug() << "---";
	for (int i = 0; i < variables.size(); i++) {
		qDebug() << variables[i].name;
	}
	*/
}


QPIEvaluatorTypes::BaseFunctions QPIEvaluatorContent::getBaseFunction(const QString & name) {
	if (name == "sin") return QPIEvaluatorTypes::bfSin;
	if (name == "cos") return QPIEvaluatorTypes::bfCos;
	if (name == "tg") return QPIEvaluatorTypes::bfTg;
	if (name == "ctg") return QPIEvaluatorTypes::bfCtg;
	if (name == "arcsin") return QPIEvaluatorTypes::bfArcsin;
	if (name == "arccos") return QPIEvaluatorTypes::bfArccos;
	if (name == "arctg") return QPIEvaluatorTypes::bfArctg;
	if (name == "arcctg") return QPIEvaluatorTypes::bfArcctg;
	if (name == "exp") return QPIEvaluatorTypes::bfExp;
	if (name == "random") return QPIEvaluatorTypes::bfRandom;
	if (name == "sh") return QPIEvaluatorTypes::bfSh;
	if (name == "ch") return QPIEvaluatorTypes::bfCh;
	if (name == "th") return QPIEvaluatorTypes::bfTh;
	if (name == "cth") return QPIEvaluatorTypes::bfCth;
	if (name == "sqrt") return QPIEvaluatorTypes::bfSqrt;
	if (name == "sqr") return QPIEvaluatorTypes::bfSqr;
	if (name == "pow") return QPIEvaluatorTypes::bfPow;
	if (name == "abs") return QPIEvaluatorTypes::bfAbs;
	if (name == "ln") return QPIEvaluatorTypes::bfLn;
	if (name == "lg") return QPIEvaluatorTypes::bfLg;
	if (name == "log") return QPIEvaluatorTypes::bfLog;
	if (name == "im") return QPIEvaluatorTypes::bfIm;
	if (name == "re") return QPIEvaluatorTypes::bfRe;
	if (name == "arg") return QPIEvaluatorTypes::bfArg;
	if (name == "len") return QPIEvaluatorTypes::bfLen;
	if (name == "conj") return QPIEvaluatorTypes::bfConj;
	if (name == "sign") return QPIEvaluatorTypes::bfSign;
	if (name == "rad") return QPIEvaluatorTypes::bfRad;
	if (name == "deg") return QPIEvaluatorTypes::bfDeg;
	return QPIEvaluatorTypes::bfUnknown;
}

const QString & QPIEvaluator::prepare(const QString & string) {
	currentString = string.trimmed();
	if (currentString.isEmpty()) currentString = "0";
	replaceOperators();
	removeSpaces();
	checkBrackets();
	while (fillElements()) checkBrackets();
	while (setSignes()) fillElements();
	removeJunk();
	findUnknownVariables();
	return currentString;
}


void QPIEvaluator::removeSpaces() {
	QString tmps = currentString;
	for (int i = 0; i < tmps.length(); i++) {
		if (tmps[i] == ' ' || tmps[i] == '\t') {
			tmps.remove(i, 1);
			i--;
		}
	}
	currentString = tmps;
}


void QPIEvaluator::removeJunk() {
	QChar cc;
	bool junk = true;
	int bcnt;
	while (junk) {
		if (currentString.left(1) != "(" || currentString.right(1) != ")") return;
		bcnt = 1;
		junk = false;
		for (int i = 1; i < currentString.length(); i++) {
			cc = currentString[i];
			if (cc == '(') bcnt++;
			if (cc == ')') bcnt--;
			if (bcnt == 0) {
				if (i == currentString.length() - 1) {
					currentString = currentString.mid(1, currentString.length() - 2);
					elements.pop_front();
					elements.pop_back();
					junk = true;
					break;
				} else break;
			}
		}
	}
}


void QPIEvaluator::replaceOperators() {
	currentString.replace("==", "=");
	currentString.replace("!=", ":");
	currentString.replace(">=", "}");
	currentString.replace("<=", "{");
	currentString.replace("&&", "&");
	currentString.replace("||", "|");
}


void QPIEvaluator::makeOutput(QString & string) {
	string.replace(":", "≠");
	string.replace("}", "≥");
	string.replace("{", "≤");
	string.replace("&", "⋀");
	string.replace("|", "⋁");
}


void QPIEvaluator::findUnknownVariables() {
	QString cvar;
	unknownVars.clear();
	for (int i = 0; i < currentString.length(); i++) {
		if (elements[i].var_num == -666) cvar += currentString[i];
		else {
			if (cvar.length() == 0) continue;
			unknownVars << cvar;
			cvar = "";
		}
	}
	if (cvar.length() > 0) unknownVars << cvar;
	unknownVars.removeDuplicates();
}


bool QPIEvaluator::isSign(const QChar & ch) {
	return ch == '+' || ch == '-' ||
		   ch == '*' || ch == '/' ||
		   ch == '%' || ch == '^' ||
		   ch == '=' || ch == ':' ||
		   ch == '>' || ch == '<' ||
		   ch == '}' || ch == '{' ||
		   ch == '&' || ch == '|';
}


void QPIEvaluator::checkBrackets() {
	QString tmps = currentString;
	QChar fc, sc;
	int bcnt = 0, bpos = 0, inserted = 0;
	currentString = tmps;
	for (int i = 0; i < tmps.length(); i++) {
		if (tmps[i] == '(') {
			if (bcnt == 0) bpos = i;
			bcnt++;
		}
		if (tmps[i] == ')') {
			if (bcnt == 0) {
				currentString.insert(bpos + inserted, "(");
				inserted++;
			} else bcnt--;
		}
	}
	if (bcnt > 0) currentString += QString(bcnt, ')');
	tmps = currentString;
	for (int i = 0; i < tmps.length() - 1; i++) {
		fc = tmps[i].toLower();
		sc = tmps[i + 1].toLower();
		if ((fc == ')' && sc == '(') ||
			(fc == ')' && sc >= '0' && sc <= '9') ||
			(fc == ')' && sc >= 'a' && sc <= 'z') ) tmps.insert(++i, '*');
	}
	currentString = tmps;
}


bool QPIEvaluator::fillElements() {
	int fstart, flen, cnum = 0, cpart = 0, cfunc;
	QChar cc, nc, pc, fc = '!';
	bool numFound = false;
	QString curfind, tmps = currentString;
	elements.resize(tmps.length());
	for (int i = 0; i < elements.size(); i++) {
		elements[i].type = QPIEvaluatorTypes::etVariable;
		elements[i].var_num = -666;
	}
	currentVariables.clear();
	//qDebug().nospace() << "search for functions ...";
	for (int i = 0; i < content.functionsCount(); i++) {
		curfind = content.function(i).identifier;
		cfunc = i; //(int)content.function(i).type;
		flen = curfind.length();
		fstart = 0;
		while (fstart >= 0) {
			fstart = tmps.indexOf(curfind, fstart);
			if (fstart < 0) break;
			if (tmps[fstart + flen] != '(') {
				//currentString.insert(fstart + flen, "(");
				fstart++;
				continue;
			}
			for (int j = fstart; j < fstart + flen; j++) {
				elements[j].set(QPIEvaluatorTypes::etFunction, cnum, cfunc);
				tmps.replace(j, 1, fc);
			}
			cnum++;
		}
	}
	cnum = 0;
	//qDebug().nospace() << "search for variables ...";
	for (int i = 0; i < content.variablesCount(); i++) {
		curfind = content.variable(i).name;
		flen = curfind.length();
		fstart = 0;
		while (fstart >= 0) {
			fstart = tmps.indexOf(curfind, fstart);
			if (fstart < 0) break;
			for (int j = fstart; j < fstart + flen; j++) {
				elements[j].set(QPIEvaluatorTypes::etVariable, cnum, i);
				tmps.replace(j, 1, fc);
			}
			cnum++;
		}
	}
	curfind = "";
	cnum = 1;
	//qDebug().nospace() << "search for numbers ...";
	for (int i = 0; i < tmps.length(); i++) {
		cc = tmps[i];
		/*if (cc == " " || cc == "(" || cc == ")") {
			curfind = "";
			cpart = 0;
			numFound = false;
			continue;
		}*/
		switch (cpart) {
		case 0:
			if ((cc >= '0' && cc <= '9')) {// || cc == '-' || cc == '+') {
				curfind += cc;
				cpart = 1;
				continue;
			}
			if (cc == '.') {
				curfind += cc;
				cpart = 2;
				continue;
			}
			if (cc == 'E') {
				curfind += cc;
				cpart = 3;
				continue;
			}
			break;
		case 1:
			if (cc >= '0' && cc <= '9') {
				curfind += cc;
				continue;
			}
			if (cc == '.') {
				curfind += cc;
				cpart = 2;
				continue;
			}
			if (cc == 'E') {
				curfind += cc;
				cpart = 3;
				continue;
			}
			numFound = true;
			break;
		case 2:
			if (cc >= '0' && cc <= '9') {
				curfind += cc;
				continue;
			}
			if (cc == 'E') {
				curfind += cc;
				cpart = 3;
				continue;
			}
			numFound = true;
			break;
		case 3:
			if ((cc >= '0' && cc <= '9') || cc == '-' || cc == '+') {
				curfind += cc;
				cpart = 4;
				continue;
			}
			numFound = true;
			break;
		case 4:
			if (cc >= '0' && cc <= '9') {
				curfind += cc;
				continue;
			}
			numFound = true;
			break;
		}
		if (numFound) {
			//qDebug().nospace() << "add " << cnum << ": " << curfind << " = " << curfind.toDouble();
			currentVariables.push_back(QPIEvaluatorTypes::Variable("tmp" + QString::number(cnum), curfind.toDouble()));
			for (int j = i - curfind.length(); j < i; j++) {
				elements[j].set(QPIEvaluatorTypes::etNumber, cnum, -cnum);
				tmps.replace(j, 1, fc);
			}
			curfind = "";
			cnum++;
			cpart = 0;
			numFound = false;
		}
	}
	if (cpart > 0) {
		//qDebug().nospace() << "add " << cnum << ": " << curfind << " = " << curfind.toDouble();
		currentVariables.push_back(QPIEvaluatorTypes::Variable("tmp" + QString::number(cnum), curfind.toDouble()));
		for (int j = tmps.length() - curfind.length(); j < tmps.length(); j++) {
			elements[j].set(QPIEvaluatorTypes::etNumber, cnum, -cnum);
			tmps.replace(j, 1, fc);
		}
	}
	cc = nc = fc;
	//qDebug().nospace() << "search for signes ...";
	for (int i = 0; i < tmps.length(); i++) {
		cc = tmps[i];
		if (i > 0) pc = tmps[i - 1];
		else pc = fc;
		if (i < tmps.length() - 1) nc = tmps[i + 1];
		else nc = fc;
		if (cc == '(' || cc == ')' || cc == ',') {
			elements[i].set(QPIEvaluatorTypes::etOperator, -1);
			continue;
		}
		if (cc == '-' || cc == '+') {
			elements[i].set(QPIEvaluatorTypes::etOperator, -1);
			if (i < tmps.length() - 1) if (elements[i + 1].type == QPIEvaluatorTypes::etVariable ||
										   elements[i + 1].type == QPIEvaluatorTypes::etFunction) continue;
			if ((pc == '(' || isSign(pc) || i == 0) && i < tmps.length() - 1) {
				if (elements[i + 1].type != QPIEvaluatorTypes::etOperator) {
					cnum = elements[i + 1].num;
					elements[i].set(QPIEvaluatorTypes::etNumber, cnum);
					tmps.replace(i, 1, fc);
					///cout << "found sign " << cc << " :" << cnum - 1 << endl;
					if (cc == '-' && currentVariables.size() >= cnum)
						currentVariables[cnum - 1].value = -currentVariables[cnum - 1].value;
					//i++;
					continue;
				}
			}
		}
		if (isSign(cc)) {
			elements[i].set(QPIEvaluatorTypes::etOperator, -1);
			continue;
		}
	}
	/*
	qDebug().nospace() << tmps;
	cout << " ";
	for (int i = 0; i < elements.size(); i++) {
		switch (elements[i].type) {
		case etFunction: cout << "f"; break;
		case etNumber: cout << "n"; break;
		case etOperator: cout << "o"; break;
		case etVariable: cout << "v"; break;
		}
	}
	cout << endl;
	*/
	return false;
	//for (int i = 0; i < currentVariables.size(); i++) qDebug() << "var " << i << ": " << currentVariables[i].value.real();
}


bool QPIEvaluator::setSignes() {
	int inserted = 0, ni, pi = 0, needInsert = 0;
	QChar fc, sc, pc;
	QString tmps = currentString;
	for (int i = 0; i < tmps.length() - 1; i++) {
		needInsert = 0;
		ni = i + 1;
		if (i > 0) pi = i - 1;
		fc = tmps[i].toLower();
		sc = tmps[ni].toLower();
		pc = tmps[pi].toLower();
		//if (elements[i].type == etOperator || elements[ni].type == etVariable) continue;
		if (fc == ',' || sc == ',') continue;
		if (elements[i].type == QPIEvaluatorTypes::etOperator && elements[ni].type == QPIEvaluatorTypes::etOperator) continue;
		if (fc == ')' && (elements[ni].type == QPIEvaluatorTypes::etNumber || elements[ni].type == QPIEvaluatorTypes::etVariable || elements[ni].type == QPIEvaluatorTypes::etFunction)) needInsert = 1;
		if (sc == '(' && (elements[i].type == QPIEvaluatorTypes::etNumber || elements[i].type == QPIEvaluatorTypes::etVariable)) needInsert = 1;
		if (elements[i].type == QPIEvaluatorTypes::etNumber && elements[ni].type == QPIEvaluatorTypes::etNumber && elements[i].num != elements[ni].num) needInsert = 1;
		if (elements[i].type == QPIEvaluatorTypes::etVariable && elements[ni].type == QPIEvaluatorTypes::etVariable && elements[i].num != elements[ni].num) needInsert = 1;
		if ((elements[i].type == QPIEvaluatorTypes::etNumber && elements[ni].type == QPIEvaluatorTypes::etVariable) || (elements[i].type == QPIEvaluatorTypes::etVariable && elements[ni].type == QPIEvaluatorTypes::etNumber)) needInsert = 1;
		if ((elements[i].type == QPIEvaluatorTypes::etNumber || elements[i].type == QPIEvaluatorTypes::etVariable) && elements[ni].type == QPIEvaluatorTypes::etFunction) needInsert = 1;
		if (elements[i].type == QPIEvaluatorTypes::etFunction && elements[ni].type == QPIEvaluatorTypes::etFunction && elements[i].num != elements[ni].num) needInsert = 2;
		if (elements[i].type == QPIEvaluatorTypes::etFunction && elements[ni].type != QPIEvaluatorTypes::etFunction && sc != '(') needInsert = 2;
		if (elements[pi].type == QPIEvaluatorTypes::etOperator && (elements[ni].type == QPIEvaluatorTypes::etFunction || elements[ni].type == QPIEvaluatorTypes::etVariable) && fc == '-') needInsert = 3;
		switch (needInsert) {
		case 1:
			currentString.insert(ni + inserted, "*");
			elements.insert(ni + inserted, QPIEvaluatorTypes::Element(QPIEvaluatorTypes::etOperator, -1));
			//inserted++;
			//i++;
			return true;
		/*case 2:
			currentString.insert(ni + inserted, ")");
			currentString.insert(ni + inserted, "(");
			elements.insert(ni + inserted, Element(etOperator, -1));
			elements.insert(ni + inserted, Element(etOperator, -1));
			inserted++;
			i++;
			return true;*/
		case 3:
			currentString.insert(ni + inserted, "1*");
			elements.insert(ni + inserted, QPIEvaluatorTypes::Element(QPIEvaluatorTypes::etOperator, -1));
			//inserted;
			//i++;
			return true;
		}
	}
	/*if (elements[tmps.length() - 1].type == etFunction) {
		currentString.insert(tmps.length() + inserted, ")");
		currentString.insert(tmps.length() + inserted, "(");
		elements.insert(tmps.length() + inserted, Element(etOperator, -1));
		elements.insert(tmps.length() + inserted, Element(etOperator, -1));
		return true;
	}*/
	return false;
}


void QPIEvaluator::convert() {
	int j;
	QPIEvaluatorTypes::Element ce, pe;
	for (int i = 0; i < currentString.length(); i++) {
		pe = elements[i];
		if (pe.type != QPIEvaluatorTypes::etFunction) continue;
		j = i + 1;
		while (j < currentString.length()) {
			ce = elements[j];
			if (ce != pe) break;
			j++;
		}
		currentString.replace(i, j - i, " ");
		for (int k = i + 1; k < j; k++) elements.remove(i);
		//i++;
	}
	for (int i = 0; i < currentString.length(); i++) {
		pe = elements[i];
		if (pe.type != QPIEvaluatorTypes::etNumber) continue;
		j = i + 1;
		while (j < currentString.length()) {
			ce = elements[j];
			if (ce != pe) break;
			j++;
		}
		currentString.replace(i, j - i, " ");
		for (int k = i + 1; k < j; k++) elements.remove(i);
		//i++;
	}
	for (int i = 0; i < currentString.length(); i++) {
		pe = elements[i];
		if (pe.type != QPIEvaluatorTypes::etVariable) continue;
		j = i + 1;
		while (j < currentString.length()) {
			ce = elements[j];
			if (ce != pe) break;
			j++;
		}
		currentString.replace(i, j - i, " ");
		for (int k = i + 1; k < j; k++) elements.remove(i);
		//i++;
	}
	/*qDebug().nospace() << currentString;
	cout << " ";
	for (int i = 0; i < elements.size(); i++) {
		switch (elements[i].type) {
		case etFunction: cout << "f"; break;
		case etNumber: cout << "n"; break;
		case etOperator: cout << "o"; break;
		case etVariable: cout << "v"; break;
		}
	}
	cout << endl;*/
}



const QString & QPIEvaluator::preprocess(const QString & string) {
	static QString ret;
	int lind;
	ret = prepare(string);
	convert();
	instructions.clear();
	//qDebug() << preproc->currentString;
	variables = currentVariables;
	lind = parse(currentString);
	if (instructions.size() == 0) {
		variables.push_back(QPIEvaluatorTypes::Variable());
		instructions.push_back(QPIEvaluatorTypes::Instruction(QPIEvaluatorTypes::oNone, QVector<int>(1, lind), -variables.size()));
	}
	kvars = &(content.variables);
	/*
	cout << endl << "variables:" << endl;
	for (int i = 0; i < variables.size(); i++)
		cout << i << " value = " << variables[i].value << endl;

	cout << endl << "instructions:" << endl;
	for (int i = 0; i < instructions.size(); i++) {
		cout << i << endl;
		cout << " operation " << instructions[i].operation << endl;
		cout << " operators: ";
		for (int j = 0; j < instructions[i].operators.size(); j++)
			cout << instructions[i].operators[j] << "; ";
		cout << endl << " function " << instructions[i].function << endl;
		cout << " out " << instructions[i].out << endl;
	}
	*/
	makeOutput(ret);
	return ret;
}


QPIEvaluatorTypes::Operation QPIEvaluator::operationInOrder(const int & index) {
	switch (index) {
	case 0: return QPIEvaluatorTypes::oPower;
	case 1: return QPIEvaluatorTypes::oMultiply;
	case 2: return QPIEvaluatorTypes::oDivide;
	case 3: return QPIEvaluatorTypes::oResidue;
	case 4: return QPIEvaluatorTypes::oAdd;
	case 5: return QPIEvaluatorTypes::oSubtract;
	case 6: return QPIEvaluatorTypes::oEqual;
	case 7: return QPIEvaluatorTypes::oNotEqual;
	case 8: return QPIEvaluatorTypes::oGreaterEqual;
	case 9: return QPIEvaluatorTypes::oSmallerEqual;
	case 10: return QPIEvaluatorTypes::oGreater;
	case 11: return QPIEvaluatorTypes::oSmaller;
	case 12: return QPIEvaluatorTypes::oAnd;
	case 13: return QPIEvaluatorTypes::oOr;
	default: return QPIEvaluatorTypes::oNone;
	}
}


int QPIEvaluator::parse(const QString & string, int offset) {
	int slen = string.length(), /*facnt,*/ farg, bcnt, k;
	QChar cc;
	QPIEvaluatorTypes::Element ce;
	QPIEvaluatorTypes::Function cfunc;
	QPIEvaluatorTypes::Operation coper;
	QString sbrackets, carg;
	QVector<int> args, atmp;
	QVector<QPIEvaluatorTypes::Operation> opers;

	///qDebug() << "to parse :" + string;
	///cout << "           "; for (int i = 0; i < slen; i++) cout << preproc->elements[i + offset].type; cout << endl;

	for (int i = 0; i < slen; i++) {
		ce = elements[i + offset];
		cc = string[i];
		switch (ce.type) {
		case QPIEvaluatorTypes::etNumber:
			args.push_back(ce.var_num);
			continue;
		case QPIEvaluatorTypes::etVariable:
			args.push_back(ce.var_num);
			continue;
		case QPIEvaluatorTypes::etFunction:
			i++;
			cfunc = content.function(ce.var_num);
			//facnt = cfunc.arguments;
			atmp.clear();
			bcnt = farg = 1;
			///qDebug() << "function: " + cfunc.identifier;
			//for (int k = 0; k < facnt; k++) {
			carg = "";
			k = i + 1;
			//if (string.size() <= k || k < 0) return -666;
			while (bcnt > 0) {
				//if (k < facnt - 1) fcomma = string.indexOf(',', j);
				cc = string[k];
				switch (cc.toLatin1()) {
				case '(': bcnt++; break;
				case ')':
					bcnt--;
					if (bcnt == 0) {
						///qDebug() << "arument: " << carg;
						atmp.push_back(parse(carg, k + offset - carg.length()));
						k++;
						carg = "";
						if (atmp.size() > 0) if (atmp.back() < 0 && farg > 0) farg = atmp.back();
						continue;
					}
					break;
				case ',':
					if (bcnt == 1) {
						///qDebug() << "arument: " << carg;
						atmp.push_back(parse(carg, k + offset - carg.length()));
						k++;
						carg = "";
						if (atmp.size() > 0) if (atmp.back() < 0 && farg > 0) farg = atmp.back();
						continue;
					}
					break;
				}
				carg += cc;
				k++;
			}
			i = k - 1;
			if (farg > 0) {
				variables.push_back(QPIEvaluatorTypes::Variable());
				farg = -variables.size();
			}
			instructions.push_back(QPIEvaluatorTypes::Instruction(QPIEvaluatorTypes::oFunction, atmp, farg, ce.var_num));
			args.push_back(farg);
			//for (int i = 0; i < args.size(); i++) cout << preproc->currentVariables[-args[i]].value << endl;
			//i = j + 1;
			continue;
		case QPIEvaluatorTypes::etOperator:
			//qDebug() << "operator: " << cc;
			if (cc == '(') {
				sbrackets = inBrackets(string.right(slen - i));
				args.push_back(parse(sbrackets, i + offset + 1));
				i += sbrackets.length() + 1;
				continue;
			}
			if (cc == '+') {opers.push_back(QPIEvaluatorTypes::oAdd); continue;}
			if (cc == '-') {opers.push_back(QPIEvaluatorTypes::oSubtract); continue;}
			if (cc == '*') {opers.push_back(QPIEvaluatorTypes::oMultiply); continue;}
			if (cc == '/') {opers.push_back(QPIEvaluatorTypes::oDivide); continue;}
			if (cc == '%') {opers.push_back(QPIEvaluatorTypes::oResidue); continue;}
			if (cc == '^') {opers.push_back(QPIEvaluatorTypes::oPower); continue;}
			if (cc == '=') {opers.push_back(QPIEvaluatorTypes::oEqual); continue;}
			if (cc == ':') {opers.push_back(QPIEvaluatorTypes::oNotEqual); continue;}
			if (cc == '}') {opers.push_back(QPIEvaluatorTypes::oGreaterEqual); continue;}
			if (cc == '{') {opers.push_back(QPIEvaluatorTypes::oSmallerEqual); continue;}
			if (cc == '>') {opers.push_back(QPIEvaluatorTypes::oGreater); continue;}
			if (cc == '<') {opers.push_back(QPIEvaluatorTypes::oSmaller); continue;}
			if (cc == '&') {opers.push_back(QPIEvaluatorTypes::oAnd); continue;}
			if (cc == '|') {opers.push_back(QPIEvaluatorTypes::oOr); continue;}
		}
	}
	/*
	cout << "stack: " << endl << "args: ";
	for (int i = 0; i < args.size(); i++) cout << args[i] << ", ";
	cout << endl << "opers: ";
	for (int i = 0; i < opers.size(); i++) cout << opers[i] << ", ";
	*/
	if (opers.size() == 0) {
		if (args.size() > 0) return args.back();
		else return -666;
	}
	for (int i = 0; i < QPIEvaluatorTypes::operationCount; i++) {
		coper = operationInOrder(i);
		for (int j = 0; j < opers.size(); j++) {
			if (coper == QPIEvaluatorTypes::oDivide || coper == QPIEvaluatorTypes::oMultiply) {
				if (opers[j] != QPIEvaluatorTypes::oDivide && opers[j] != QPIEvaluatorTypes::oMultiply) continue;
			} else {
				if (opers[j] != coper) continue;
			}
			atmp.clear();
			if (j < args.size() && j >= 0) atmp.push_back(args[j]);
			else atmp.push_back(-666);
			if (j + 1 < args.size() && j >= -1) atmp.push_back(args[j + 1]);
			else atmp.push_back(-666);
			farg = 1;
			if (atmp[0] < 0) farg = atmp[0];
			else {
				if (atmp[1] < 0) farg = atmp[1];
				else {
					variables.push_back(QPIEvaluatorTypes::Variable());
					farg = -variables.size();
				}
			}
			instructions.push_back(QPIEvaluatorTypes::Instruction(opers[j], atmp, farg));
			if (j >= 0 && j < args.size()) {
				args.remove(j);
				if (j < args.size()) args[j] = farg;
			}
			opers.remove(j);
			j--;
		}
	}
	return instructions.back().out;
	///cout << endl;
}


bool QPIEvaluator::check() {
	QPIEvaluatorTypes::Instruction ci;
	bool error;
	if (unknownVars.size() > 0) {
		lastError = "Unknown variables: \"" + unknownVars.join("\", \"") + "\"";
		return false;
	}
	for (int i = 0; i < instructions.size(); i++) {
		error = false;
		ci = instructions[i];
		switch (ci.operation) {
		case QPIEvaluatorTypes::oNone: break;
		case QPIEvaluatorTypes::oFunction:
			for (int j = 0; j < ci.operators.size(); j++) {
				if (ci.operators[j] == -666) { //(ci.operators[j] < -variables.size() || ci.operators[j] >= kvars->size()) {
					error = true;
					break;
				}
			}
			if (ci.operators.size() != content.function(ci.function).arguments || error) {
				lastError = "Invalid arguments count for function \"" + content.function(ci.function).identifier + "\"";
				return false;
			}
			break;
		default:
			if (ci.operators[0] == -666 || ci.operators[1] == -666) error = true;
			if (ci.operators.size() != 2 || error) {
				lastError = "Invalid arguments count for operation \" " + operationChar(ci.operation) + " \"";
				return false;
			}
			break;
		}
		if (ci.out < -variables.size()) {
			lastError = "Invalid variable index \"" + QString::number(ci.out) + "\"";
			return false;
		}
		for (int j = 0; j < ci.operators.size(); j++) {
			if (ci.operators[j] < -variables.size() || ci.operators[j] >= kvars->size()) {
				lastError = "Invalid variable index \"" + QString::number(ci.operators[j]) + "\"";
				return false;
			}
		}
	}
	return true;
}


QString QPIEvaluator::inBrackets(const QString & string) {
	int slen = string.length(), bcnt = 0;
	QChar cc;
	for (int i = 0; i < slen; i++) {
		cc = string[i];
		if (cc == '(') bcnt++;
		if (cc == ')') {
			bcnt--;
			if (bcnt == 0) return string.mid(1, i - 1);
		}
	}
	return QString();
}


QString QPIEvaluator::operationChar(const QPIEvaluatorTypes::Operation & operation) {
	switch (operation) {
	case QPIEvaluatorTypes::oAdd: return "+";
	case QPIEvaluatorTypes::oSubtract: return "-";
	case QPIEvaluatorTypes::oMultiply: return "*";
	case QPIEvaluatorTypes::oDivide: return "/";
	case QPIEvaluatorTypes::oPower: return "^";
	case QPIEvaluatorTypes::oResidue: return "%";
	case QPIEvaluatorTypes::oEqual: return "=";
	case QPIEvaluatorTypes::oNotEqual: return ("≠");
	case QPIEvaluatorTypes::oGreaterEqual: return ("≥");
	case QPIEvaluatorTypes::oSmallerEqual: return ("≤");
	case QPIEvaluatorTypes::oGreater: return ">";
	case QPIEvaluatorTypes::oSmaller: return "<";
	case QPIEvaluatorTypes::oAnd: return ("⋀");
	case QPIEvaluatorTypes::oOr: return ("⋁");
	default: return "???";
	}
}


inline complexd QPIEvaluator::residue(const complexd & f, const complexd & s) {
	complexd ret;
	if (s.real() != 0.) ret = complexd(f.real() - ((int)(f.real() / s.real())) * s.real(), 0.);
	if (s.imag() != 0.) ret = complexd(ret.real(), f.imag() - ((int)(f.imag() / s.imag())) * s.imag());
	return ret;
}


inline void QPIEvaluator::execFunction(const QPIEvaluatorTypes::Instruction & ci) {
	QPIEvaluatorTypes::Function cfunc = content.function(ci.function);
	int oi = -ci.out - 1;
	complexd tmp, stmp;
	double ldtmp;
	//qDebug() << "function " << (int)cfunc.type;
	switch (cfunc.type) {
	case QPIEvaluatorTypes::bfSin:
		tmpvars[oi].value = sin(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfCos:
		tmpvars[oi].value = cos(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfTg:
		tmpvars[oi].value = tan(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfCtg:
		tmp = tan(value(ci.operators[0]));
		if (tmp == complexd_0) tmpvars[oi].value = 0.;
		else tmpvars[oi].value = complexd_1 / tmp;
		break;
	case QPIEvaluatorTypes::bfArcsin:
		tmpvars[oi].value = asinc_qpie(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfArccos:
		tmpvars[oi].value = acosc_qpie(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfArctg:
		tmpvars[oi].value = atanc_qpie(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfArcctg:
		tmp = atanc_qpie(value(ci.operators[0]));
		if (tmp == complexd_0) tmpvars[oi].value = 0.;
		else tmpvars[oi].value = complexd_1 / tmp;
		break;
	case QPIEvaluatorTypes::bfSh:
		tmpvars[oi].value = sinh(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfCh:
		tmpvars[oi].value = cosh(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfTh:
		tmpvars[oi].value = tanh(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfCth:
		tmp = tanh(value(ci.operators[0]));
		if (tmp == complexd_0) tmpvars[oi].value = 0.;
		else tmpvars[oi].value = complexd_1 / tmp;
		break;
	case QPIEvaluatorTypes::bfAbs:
		tmpvars[oi].value = abs(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfSqrt:
		tmpvars[oi].value = sqrt(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfSqr:
		tmpvars[oi].value = value(ci.operators[0]) * value(ci.operators[0]);
		break;
	case QPIEvaluatorTypes::bfExp:
		tmpvars[oi].value = exp(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfPow:
		tmpvars[oi].value = pow(value(ci.operators[0]), value(ci.operators[1]));
		break;
	case QPIEvaluatorTypes::bfLn:
		tmpvars[oi].value = log(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfLg:
		tmpvars[oi].value = log10(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfLog:
		tmp = log(value(ci.operators[1]));
		if (tmp == complexd_0) tmpvars[oi].value = 0.;
		else tmpvars[oi].value = log(value(ci.operators[0])) / tmp;
		break;
	case QPIEvaluatorTypes::bfRe:
		tmpvars[oi].value = value(ci.operators[0]).real();
		break;
	case QPIEvaluatorTypes::bfIm:
		tmpvars[oi].value = value(ci.operators[0]).imag();
		break;
	case QPIEvaluatorTypes::bfArg:
		tmpvars[oi].value = arg(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfLen:
		tmpvars[oi].value = abs(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfConj:
		tmpvars[oi].value = conj(value(ci.operators[0]));
		break;
	case QPIEvaluatorTypes::bfSign:
		ldtmp = value(ci.operators[0]).real();
		tmpvars[oi].value = ldtmp >= 0. ? complexd_1 : -complexd_1;
		break;
	case QPIEvaluatorTypes::bfRad:
		tmpvars[oi].value = value(ci.operators[0]) * complexd(deg2rad_qpie, 0.);
		break;
	case QPIEvaluatorTypes::bfDeg:
		tmpvars[oi].value = value(ci.operators[0]) * complexd(rad2deg_qpie, 0.);
		break;
	case QPIEvaluatorTypes::bfRandom:
		tmp = static_cast<double>(qrand()) / RAND_MAX;
		stmp = value(ci.operators[1]) - value(ci.operators[0]);
		tmpvars[oi].value = value(ci.operators[0]) + tmp * stmp;
		break;
	default: break;
	}
}


inline bool QPIEvaluator::execInstructions() {
	QPIEvaluatorTypes::Instruction ci;
	int oi;
	complexd tmp;
	tmpvars = variables;
	//cout << "var count " << tmpvars.size() << endl;
	for (int i = 0; i < instructions.size(); i++) {
		ci = instructions[i];
		oi = -ci.out - 1;
		//cout << value(ci.operators[0]) << operationChar(ci.operation) << value(ci.operators[1]) << ", " << oi << endl;
		switch (ci.operation) {
		case QPIEvaluatorTypes::oAdd:
			tmpvars[oi].value = value(ci.operators[0]) + value(ci.operators[1]);
			break;
		case QPIEvaluatorTypes::oSubtract:
			tmpvars[oi].value = value(ci.operators[0]) - value(ci.operators[1]);
			break;
		case QPIEvaluatorTypes::oMultiply:
			tmpvars[oi].value = value(ci.operators[0]) * value(ci.operators[1]);
			break;
		case QPIEvaluatorTypes::oDivide:
			tmp = value(ci.operators[1]);
			if (tmp == complexd(0., 0.)) tmpvars[oi].value = 0.;
			else tmpvars[oi].value = value(ci.operators[0]) / tmp;
			break;
		case QPIEvaluatorTypes::oResidue:
			tmpvars[oi].value = residue(value(ci.operators[0]), value(ci.operators[1]));
			break;
		case QPIEvaluatorTypes::oPower:
			tmpvars[oi].value = pow(value(ci.operators[0]), value(ci.operators[1]));
			break;
		case QPIEvaluatorTypes::oEqual:
			tmpvars[oi].value = value(ci.operators[0]) == value(ci.operators[1]);
			break;
		case QPIEvaluatorTypes::oNotEqual:
			tmpvars[oi].value = value(ci.operators[0]) != value(ci.operators[1]);
			break;
		case QPIEvaluatorTypes::oGreaterEqual:
			tmpvars[oi].value = value(ci.operators[0]).real() >= value(ci.operators[1]).real();
			break;
		case QPIEvaluatorTypes::oSmallerEqual:
			tmpvars[oi].value = value(ci.operators[0]).real() <= value(ci.operators[1]).real();
			break;
		case QPIEvaluatorTypes::oGreater:
			tmpvars[oi].value = value(ci.operators[0]).real() > value(ci.operators[1]).real();
			break;
		case QPIEvaluatorTypes::oSmaller:
			tmpvars[oi].value = value(ci.operators[0]).real() < value(ci.operators[1]).real();
			break;
		case QPIEvaluatorTypes::oAnd:
			tmpvars[oi].value = value(ci.operators[0]).real() > 0. && value(ci.operators[1]).real() > 0.;
			break;
		case QPIEvaluatorTypes::oOr:
			tmpvars[oi].value = value(ci.operators[0]).real() > 0. || value(ci.operators[1]).real() > 0.;
			break;
		case QPIEvaluatorTypes::oFunction:
			execFunction(ci);
			break;
		case QPIEvaluatorTypes::oNone:
			tmpvars[oi].value = value(ci.operators[0]);
			break;
		}
	}
	if (!instructions.isEmpty())
		out = value(instructions.back().out);
	return true;
}


bool QPIEvaluator::check(const QString & string) {
	currentString = preprocess(string);
	correct = check();
	if (!correct)
		return false;
	lastError = "Correct";
	return true;
}


int QPIEvaluator::setVariable(const QString & name, complexd value) {
	int i = content.findVariable(name);
	if (i < 0) {
		content.addVariable(name, value);
		return content.findVariable(name);
	} else {
		content.setVariableValue(i, value);
		return i;
	}
	return -1;
}


complexd QPIEvaluator::evaluate() {
	if (!execInstructions()) out = 0.;
	if (fabs(out.real()) < 1E-300) out = complexd(0., out.imag());
	if (fabs(out.imag()) < 1E-300) out = complexd(out.real(), 0.);
	return out;
}
