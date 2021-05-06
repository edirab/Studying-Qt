/*! \file pifixedpoint.h
 * \brief Class for fixed point numbers
*/
/*
	PIP - Platform Independent Primitives
	Class for fixed point numbers
	Copyright (C) 2016  Andrey Bychkov work.a.b@yandex.ru

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

#include "picout.h"

#ifndef PIFIXEDPOINT_H
#define PIFIXEDPOINT_H
template<int Precision = 0, typename Type = int>
class PIFixedPoint {
//	friend PICout operator <<(PICout s, const FixedPoint<> & v);
public:
	typedef PIFixedPoint<Precision, Type> fp;
	PIFixedPoint(const Type &v = Type()) {val = fpv(v);}
	PIFixedPoint(const fp &v) {val = v.val;}
	PIFixedPoint(const float &v) {val = Precision == 0 ? Type(v) : Type(v * (2 << Precision-1));}
	PIFixedPoint(const double &v) {val = Precision == 0 ? Type(v) : Type(v * (2 << Precision-1));}
//	FixedPoint(const long double &v) {val = Precision == 0 ? Type(v) : Type(v * (2 << Precision-1));}

	template<typename T>
	fp & operator =(const T & v) {val = fpv(Type(v)); return *this;}
	fp & operator =(const fp & v) {val = v.val; return *this;}
	fp & operator =(const float &v) {val = PIFixedPoint(v).val; return *this;}
	fp & operator =(const double &v) {val = PIFixedPoint(v).val; return *this;}
	fp & operator =(const long double &v) {val = PIFixedPoint(v).val; return *this;}
		fp operator -() {fp p = fp(*this); p.val = -val; return p;}
	bool operator ==(const fp & v) const {return val == v.val;}
	bool operator !=(const fp & v) const {return val != v.val;}

	void operator +=(const fp & v) {val += v.val;}
	void operator -=(const fp & v) {val -= v.val;}

	void operator *=(const fp & v) {val = fpi(val *v.val);}
	void operator /=(const fp & v) {val = fpv(val) / v.val;}

	fp operator +(const fp & v) {fp p = fp(*this); p.val += v.val; return p;}
	fp operator -(const fp & v) {fp p = fp(*this); p.val -= v.val; return p;}

	fp operator *(const fp & v) {fp p; p.val = fpi(val * v.val); return p;}
	fp operator /(const fp & v) {fp p; p.val = fpv(val) / v.val; return p;}

	/*fp & operator =(const Type & v) {val = fpv(v); return *this;}
	bool operator ==(const Type & v) const {val == fpv(v);}
	bool operator !=(const Type & v) const {val != fpv(v);}
	void operator +=(const Type & v) {val += fpv(v);}
	void operator -=(const Type & v) {val -= fpv(v);}
	void operator *=(const Type & v) {val *= fpv(v);}
	void operator /=(const Type & v) {val /= fpv(v);}
	fp operator +(const Type & v) {fp p = fp(*this); p.val += fpv(v); return p;}
	fp operator -(const Type & v) {fp p = fp(*this); p.val -= fpv(v); return p;}
	fp operator *(const Type & v) {fp p = fp(*this); p.val *= fpv(v); return p;}
	fp operator /(const Type & v) {fp p = fp(*this); p.val /= fpv(v); return p;}*/

	Type fpv(Type v) const {return Type(v << Precision);}
	Type fpi(Type v) const {return Type(v >> Precision);}
	Type fpc(Type v) const {return v - fpv(fpi(v));}
	Type val;
};


template<int Precision, typename Type>
inline PICout operator <<(PICout s, const PIFixedPoint<Precision, Type> & v) {
	s.space(); s.setControl(0, true);
	if (Precision == 0) s << v.val;
	else {
		std::stringstream ss,sr;
		Type tmp = 10;
		int n = 1;
		Type rs = (2 << Precision-1);
		while(tmp < rs) tmp = tmp*10, n++;
		tmp *= 10; n++;
		Type rv = v.fpc(v.val);
		if (rv != 0) tmp = tmp / (rs / rv);
		ss << tmp;
		PIString r = ss.str();
		if (rv == 0) r.pop_front();
		else r.expandLeftTo(n,'0');
		sr << v.fpi(v.val);
		s << PIString(sr.str()) + "." + r;
	}
	s.restoreControl();
	return s;
}

#endif // PIFIXEDPOINT_H
