/*! \file pimathvector.h
 * \brief PIMathVector
*/
/*
	PIP - Platform Independent Primitives
	PIMathVector
	Copyright (C) 2016  Ivan Pelipenko peri4ko@yandex.ru, Andrey Bychkov work.a.b@yandex.ru

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

#ifndef PIMATHVECTOR_H
#define PIMATHVECTOR_H

#include "pimathbase.h"

template<uint Cols, uint Rows, typename Type>
class PIMathMatrixT;


/// Vector templated

#define PIMV_FOR(v, s) for (uint v = s; v < Size; ++v)

//#pragma pack(push, 1)
template<uint Size, typename Type = double>
class PIP_EXPORT PIMathVectorT {
	typedef PIMathVectorT<Size, Type> _CVector;
public:
	PIMathVectorT() {resize();}
	//PIMathVectorT(Type val) {resize(Size); PIMV_FOR(i, 0) c[i] = val;}
	PIMathVectorT(Type fval, ...) {resize(); c[0] = fval; va_list vl; va_start(vl, fval); PIMV_FOR(i, 1) c[i] = va_arg(vl, Type); va_end(vl);}
	PIMathVectorT(const PIVector<Type> & val) {resize(); PIMV_FOR(i, 0) c[i] = val[i];}
	PIMathVectorT(const _CVector & st, const _CVector & fn) {resize(); set(st, fn);}

	uint size() const {return Size;}
	_CVector & fill(const Type & v) {PIMV_FOR(i, 0) c[i] = v; return *this;}
	_CVector & set(Type fval, ...) {c[0] = fval; va_list vl; va_start(vl, fval); PIMV_FOR(i, 1) c[i] = va_arg(vl, Type); va_end(vl); return *this;}
	_CVector & set(const _CVector & st, const _CVector & fn) {PIMV_FOR(i, 0) c[i] = fn[i] - st[i]; return *this;}
	_CVector & move(const Type & v) {PIMV_FOR(i, 0) c[i] += v; return *this;}
	_CVector & move(const _CVector & v) {PIMV_FOR(i, 0) c[i] += v[i]; return *this;}
	_CVector & move(Type fval, ...) {c[0] += fval; va_list vl; va_start(vl, fval); PIMV_FOR(i, 1) c[i] += va_arg(vl, Type); va_end(vl); return *this;}
	Type lengthSqr() const {Type tv(0); PIMV_FOR(i, 0) tv += (c[i] * c[i]); return tv;}
	Type length() const {return sqrt(lengthSqr());}
	Type manhattanLength() const {Type tv(0); PIMV_FOR(i, 0) tv += fabs(c[i]); return tv;}
	Type angleCos(const _CVector & v) const {Type tv = v.length() * length(); return (tv == Type(0) ? Type(0) : ((*this) ^ v) / tv);}
	Type angleSin(const _CVector & v) const {Type tv = angleCos(v); return sqrt(Type(1) - tv * tv);}
	Type angleRad(const _CVector & v) const {return acos(angleCos(v));}
	Type angleDeg(const _CVector & v) const {return toDeg(acos(angleCos(v)));}
	Type angleElevation(const _CVector & v) const {_CVector z = v - *this; double c = z.angleCos(*this); return 90.0 - acos(c) * rad2deg;}
	_CVector projection(const _CVector & v) {Type tv = v.length(); return (tv == Type(0) ? _CVector() : v * (((*this) ^ v) / tv));}
	_CVector & normalize() {Type tv = length(); if (tv == Type(1)) return *this; if (piAbs<Type>(tv) <= Type(1E-100)) {fill(Type(0)); return *this;} PIMV_FOR(i, 0) c[i] /= tv; return *this;}
	_CVector normalized() {_CVector tv(*this); tv.normalize(); return tv;}
	_CVector cross(const _CVector & v) {return (*this) * v;}
	Type dot(const _CVector & v) const {return (*this) ^ v;}
	bool isNull() const {PIMV_FOR(i, 0) if (c[i] != Type(0)) return false; return true;}
	bool isOrtho(const _CVector & v) const {return ((*this) ^ v) == Type(0);}

	Type & at(uint index) {return c[index];}
	Type at(uint index) const {return c[index];}
	Type & operator [](uint index) {return c[index];}
	Type operator [](uint index) const {return c[index];}
	_CVector & operator =(const _CVector & v) {memcpy(c, v.c, sizeof(Type) * Size); return *this;}
	_CVector & operator =(const Type & v) {PIMV_FOR(i, 0) c[i] = v; return *this;}
	bool operator ==(const _CVector & v) const {PIMV_FOR(i, 0) if (c[i] != v[i]) return false; return true;}
	bool operator !=(const _CVector & v) const {return !(*this == c);}
	void operator +=(const _CVector & v) {PIMV_FOR(i, 0) c[i] += v[i];}
	void operator -=(const _CVector & v) {PIMV_FOR(i, 0) c[i] -= v[i];}
	void operator *=(const Type & v) {PIMV_FOR(i, 0) c[i] *= v;}
	void operator *=(const _CVector & v) {PIMV_FOR(i, 0) c[i] *= v[i];}
	void operator /=(const Type & v) {PIMV_FOR(i, 0) c[i] /= v;}
	void operator /=(const _CVector & v) {PIMV_FOR(i, 0) c[i] /= v[i];}
	_CVector operator -() const {_CVector tv; PIMV_FOR(i, 0) tv[i] = -c[i]; return tv;}
	_CVector operator +(const _CVector & v) const {_CVector tv = _CVector(*this); PIMV_FOR(i, 0) tv[i] += v[i]; return tv;}
	_CVector operator -(const _CVector & v) const {_CVector tv = _CVector(*this); PIMV_FOR(i, 0) tv[i] -= v[i]; return tv;}
	_CVector operator *(const Type & v) const {_CVector tv = _CVector(*this); PIMV_FOR(i, 0) tv[i] *= v; return tv;}
	_CVector operator /(const Type & v) const {_CVector tv = _CVector(*this); PIMV_FOR(i, 0) tv[i] /= v; return tv;}
	_CVector operator /(const _CVector & v) const {_CVector tv = _CVector(*this); PIMV_FOR(i, 0) tv[i] /= v[i]; return tv;}
	_CVector operator *(const _CVector & v) const {if (Size != 3) return _CVector(); _CVector tv; tv.fill(Type(1)); tv[0] = c[1]*v[2] - v[1]*c[2]; tv[1] = v[0]*c[2] - c[0]*v[2]; tv[2] = c[0]*v[1] - v[0]*c[1]; return tv;}
	_CVector operator &(const _CVector & v) const {_CVector tv = _CVector(*this); PIMV_FOR(i, 0) tv[i] *= v[i]; return tv;}
	Type operator ^(const _CVector & v) const {Type tv(0); PIMV_FOR(i, 0) tv += c[i] * v[i]; return tv;}
	
	PIMathMatrixT<1, Size, Type> transposed() const {
		PIMathMatrixT<1, Size, Type> ret;
		PIMV_FOR(i, 0) ret[0][i] = c[i];
		return ret;
	}

//	operator PIMathMatrixT<1, Size, Type>() {return transposed();}
//	operator PIMathMatrixT<Size, 1, Type>() {
//		PIMathMatrixT<Size, 1, Type> ret;
//		PIMV_FOR(i, 0) ret[i][0] = c[i];
//		return ret;
//	}

	Type distToLine(const _CVector & lp0, const _CVector & lp1) {
		_CVector a(lp0, lp1), b(lp0, *this), c(lp1, *this);
		Type f = fabs(a[0]*b[1] - a[1]*b[0]) / a.length();//, s = b.length() + c.length() - a.length();
		return f;}

	template<uint Size1, typename Type1> /// vector {Size, Type} to vector {Size1, Type1}
	PIMathVectorT<Size1, Type1> turnTo() const {PIMathVectorT<Size1, Type1> tv; uint sz = piMin<uint>(Size, Size1); for (uint i = 0; i < sz; ++i) tv[i] = c[i]; return tv;}

	static _CVector filled(const Type & v) {_CVector vv; PIMV_FOR(i, 0) vv[i] = v; return vv;}

private:
	void resize(const Type & new_value = Type()) {for (int i = 0; i < Size; ++i) c[i] = new_value;}
	
	Type c[Size];

};
//#pragma pack(pop)

template<uint Size, typename Type>
inline PIMathVectorT<Size, Type> operator *(const Type & x, const PIMathVectorT<Size, Type> & v) {
	return v * x;
}

template<uint Size, typename Type>
inline std::ostream & operator <<(std::ostream & s, const PIMathVectorT<Size, Type> & v) {s << '{'; PIMV_FOR(i, 0) {s << v[i]; if (i < Size - 1) s << ", ";} s << '}'; return s;}
template<uint Size, typename Type>
inline PICout operator <<(PICout s, const PIMathVectorT<Size, Type> & v) {s << '{'; PIMV_FOR(i, 0) {s << v[i]; if (i < Size - 1) s << ", ";} s << '}'; return s;}
template<uint Size, typename Type>
inline bool operator ||(const PIMathVectorT<Size, Type> & f, const PIMathVectorT<Size, Type> & s) {return (f * s).isNull();}
template<uint Size, typename Type>
inline PIMathVectorT<Size, Type> sqrt(const PIMathVectorT<Size, Type> & v) {PIMathVectorT<Size, Type> ret; PIMV_FOR(i, 0) {ret[i] = sqrt(v[i]);} return ret;}
template<uint Size, typename Type>
inline PIMathVectorT<Size, Type> sqr(const PIMathVectorT<Size, Type> & v) {PIMathVectorT<Size, Type> ret; PIMV_FOR(i, 0) {ret[i] = sqr(v[i]);} return ret;}

template<uint Size, typename Type>
inline PIByteArray & operator <<(PIByteArray & s, const PIMathVectorT<Size, Type> & v) {for (uint i = 0; i < Size; ++i) s << v[i]; return s;}
template<uint Size, typename Type>
inline PIByteArray & operator >>(PIByteArray & s, PIMathVectorT<Size, Type> & v) {for (uint i = 0; i < Size; ++i) s >> v[i]; return s;}

//template<uint Size0, typename Type0 = double, uint Size1 = Size0, typename Type1 = Type0> /// vector {Size0, Type0} to vector {Size1, Type1}
//inline operator PIMathVectorT<Size1, Type1>(const PIMathVectorT<Size0, Type0> & v) {PIMathVectorT<Size1, Type1> tv; uint sz = piMin<uint>(Size0, Size1); for (uint i = 0; i < sz; ++i) tv[i] = v[i]; return tv;}

typedef PIMathVectorT<2u, int> PIMathVectorT2i;
typedef PIMathVectorT<3u, int> PIMathVectorT3i;
typedef PIMathVectorT<4u, int> PIMathVectorT4i;
typedef PIMathVectorT<2u, double> PIMathVectorT2d;
typedef PIMathVectorT<3u, double> PIMathVectorT3d;
typedef PIMathVectorT<4u, double> PIMathVectorT4d;

#undef PIMV_FOR

/// Vector

#define PIMV_FOR(v, s) for (uint v = s; v < size_; ++v)

template<typename Type>
class PIP_EXPORT PIMathVector {
	typedef PIMathVector<Type> _CVector;
public:
	PIMathVector(const uint size = 3) {resize(size);}
	PIMathVector(const uint size, Type fval, ...) {resize(size); c[0] = fval; va_list vl; va_start(vl, fval); PIMV_FOR(i, 1) c[i] = va_arg(vl, Type); va_end(vl);}
	PIMathVector(const PIVector<Type> & val) {resize(val.size()); PIMV_FOR(i, 0) c[i] = val[i];}
	PIMathVector(const _CVector & st, const _CVector & fn) {resize(st.size()); PIMV_FOR(i, 0) c[i] = fn[i] - st[i];}

	uint size() const {return size_;}
	_CVector & resize(uint size, const Type & new_value = Type()) {size_ = size; c.resize(size, new_value); return *this;}
	_CVector resized(uint size, const Type & new_value = Type()) {_CVector tv = _CVector(*this); tv.resize(size, new_value); return tv;}
	_CVector & fill(const Type & v) {PIMV_FOR(i, 0) c[i] = v; return *this;}
	_CVector & set(Type fval, ...) {c[0] = fval; va_list vl; va_start(vl, fval); PIMV_FOR(i, 1) c[i] = va_arg(vl, Type); va_end(vl); return *this;}
	_CVector & move(const Type & v) {PIMV_FOR(i, 0) c[i] += v; return *this;}
	_CVector & move(const _CVector & v) {PIMV_FOR(i, 0) c[i] += v[i]; return *this;}
	_CVector & move(Type fval, ...) {c[0] += fval; va_list vl; va_start(vl, fval); PIMV_FOR(i, 1) c[i] += va_arg(vl, Type); va_end(vl); return *this;}
	_CVector & swap(uint fe, uint se) {piSwap<Type>(c[fe], c[se]); return *this;}
	Type lengthSqr() const {Type tv(0); PIMV_FOR(i, 0) tv += (c[i] * c[i]); return tv;}
	Type length() const {return sqrt(lengthSqr());}
	Type manhattanLength() const {Type tv(0); PIMV_FOR(i, 0) tv += fabs(c[i]); return tv;}
	Type angleCos(const _CVector & v) const {Type tv = v.length() * length(); return (tv == Type(0) ? Type(0) : ((*this) ^ v) / tv);}
	Type angleSin(const _CVector & v) const {Type tv = angleCos(v); return sqrt(Type(1) - tv * tv);}
	Type angleRad(const _CVector & v) const {return acos(angleCos(v));}
	Type angleDeg(const _CVector & v) const {return toDeg(acos(angleCos(v)));}
	_CVector projection(const _CVector & v) {Type tv = v.length(); return (tv == Type(0) ? _CVector() : v * (((*this) ^ v) / tv));}
	_CVector & normalize() {Type tv = length(); if (tv == Type(1)) return *this; if (piAbs<Type>(tv) <= Type(1E-100)) {fill(Type(0)); return *this;} PIMV_FOR(i, 0) c[i] /= tv; return *this;}
	_CVector normalized() {_CVector tv(*this); tv.normalize(); return tv;}
	bool isNull() const {PIMV_FOR(i, 0) if (c[i] != Type(0)) return false; return true;}
	bool isOrtho(const _CVector & v) const {return ((*this) ^ v) == Type(0);}

	Type & at(uint index) {return c[index];}
	Type at(uint index) const {return c[index];}
	Type & operator [](uint index) {return c[index];}
	Type operator [](uint index) const {return c[index];}
	_CVector & operator =(const _CVector & v) {c = v.c; return *this;}
	_CVector & operator =(const Type & v) {PIMV_FOR(i, 0) c[i] = v; return *this;}
	bool operator ==(const _CVector & v) const {PIMV_FOR(i, 0) if (c[i] != v[i]) return false; return true;}
	bool operator !=(const _CVector & v) const {return !(*this == c);}
	void operator +=(const _CVector & v) {PIMV_FOR(i, 0) c[i] += v[i];}
	void operator -=(const _CVector & v) {PIMV_FOR(i, 0) c[i] -= v[i];}
	void operator *=(const Type & v) {PIMV_FOR(i, 0) c[i] *= v;}
	void operator *=(const _CVector & v) {PIMV_FOR(i, 0) c[i] *= v[i];}
	void operator /=(const Type & v) {PIMV_FOR(i, 0) c[i] /= v;}
	void operator /=(const _CVector & v) {PIMV_FOR(i, 0) c[i] /= v[i];}
	_CVector operator -() {_CVector tv; PIMV_FOR(i, 0) tv[i] = -c[i]; return tv;}
	_CVector operator +(const _CVector & v) {_CVector tv = _CVector(*this); PIMV_FOR(i, 0) tv[i] += v[i]; return tv;}
	_CVector operator -(const _CVector & v) {_CVector tv = _CVector(*this); PIMV_FOR(i, 0) tv[i] -= v[i]; return tv;}
	_CVector operator *(const Type & v) {_CVector tv = _CVector(*this); PIMV_FOR(i, 0) tv[i] *= v; return tv;}
	_CVector operator /(const Type & v) {_CVector tv = _CVector(*this); PIMV_FOR(i, 0) tv[i] /= v; return tv;}
	_CVector operator *(const _CVector & v) {if (size_ < 3) return _CVector(); _CVector tv; tv.fill(Type(1)); tv[0] = c[1]*v[2] - v[1]*c[2]; tv[1] = v[0]*c[2] - c[0]*v[2]; tv[2] = c[0]*v[1] - v[0]*c[1]; return tv;}
	Type operator ^(const _CVector & v) const {Type tv(0); PIMV_FOR(i, 0) tv += c[i] * v[i]; return tv;}

	//inline operator PIMathMatrix<1, Size, Type>() {return PIMathMatrix<1, Size, Type>(c);}
	Type distToLine(const _CVector & lp0, const _CVector & lp1) {
		_CVector a(lp0, lp1), b(lp0, *this), c(lp1, *this);
		Type f = fabs(a[0]*b[1] - a[1]*b[0]) / a.length();//, s = b.length() + c.length() - a.length();
		return f;}

	template<typename Type1>
	PIMathVector turnTo(uint size) const {PIMathVector<Type1> tv; uint sz = piMin<uint>(size_, size); for (uint i = 0; i < sz; ++i) tv[i] = c[i]; return tv;}

private:
	uint size_;
	PIVector<Type> c;

};

#undef PIMV_FOR

template<typename Type>
inline std::ostream & operator <<(std::ostream & s, const PIMathVector<Type> & v) {s << '{'; for (uint i = 0; i < v.size(); ++i) {s << v[i]; if (i < v.size() - 1) s << ", ";} s << '}'; return s;}
template<typename Type>
inline PICout operator <<(PICout s, const PIMathVector<Type> & v) {s << '{'; for (uint i = 0; i < v.size(); ++i) {s << v[i]; if (i < v.size() - 1) s << ", ";} s << '}'; return s;}

typedef PIMathVector<int> PIMathVectori;
typedef PIMathVector<double> PIMathVectord;

#endif // PIMATHVECTOR_H
