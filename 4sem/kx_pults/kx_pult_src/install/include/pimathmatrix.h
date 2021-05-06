/*! \file pimathmatrix.h
 * \brief PIMathMatrix
*/
/*
	PIP - Platform Independent Primitives
	PIMathMatrix
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

#ifndef PIMATHMATRIX_H
#define PIMATHMATRIX_H

#include "pimathvector.h"

/// Matrix templated

#define PIMM_FOR(r, c) for (uint c = 0; c < Cols; ++c) { for (uint r = 0; r < Rows; ++r) {
#define PIMM_FOR_WB(r, c) for (uint c = 0; c < Cols; ++c) for (uint r = 0; r < Rows; ++r) // without brakes
#define PIMM_FOR_I(r, c) for (uint r = 0; r < Rows; ++r) { for (uint c = 0; c < Cols; ++c) {
#define PIMM_FOR_I_WB(r, c) for (uint r = 0; r < Rows; ++r) for (uint c = 0; c < Cols; ++c) // without brakes
#define PIMM_FOR_C(v) for (uint v = 0; v < Cols; ++v)
#define PIMM_FOR_R(v) for (uint v = 0; v < Rows; ++v)

#pragma pack(push, 1)
template<uint Rows, uint Cols = Rows, typename Type = double>
class PIP_EXPORT PIMathMatrixT {
	typedef PIMathMatrixT<Rows, Cols, Type> _CMatrix;
	typedef PIMathMatrixT<Cols, Rows, Type> _CMatrixI;
	typedef PIMathVectorT<Rows, Type> _CMCol;
	typedef PIMathVectorT<Cols, Type> _CMRow;
public:
	PIMathMatrixT() {resize(Rows, Cols);}
	PIMathMatrixT(Type fval, ...) {resize(Rows, Cols); va_list vl; va_start(vl, fval); PIMM_FOR_I_WB(r, c) m[r][c] = (r + c == 0 ? fval : va_arg(vl, Type)); va_end(vl);}
	PIMathMatrixT(const PIVector<Type> & val) {resize(Rows, Cols); int i = 0; PIMM_FOR_I_WB(r, c) m[r][c] = val[i++];}
	//PIMathMatrixT(const _CMatrix & o) {resize(Rows, Cols); int i = 0; PIMM_FOR_I_WB(r, c) m[r][c] = val[i++];}

	static _CMatrix identity() {_CMatrix tm = _CMatrix(); PIMM_FOR_WB(r, c) tm.m[r][c] = (c == r ? Type(1) : Type(0)); return tm;}
	static _CMatrix filled(const Type & v) {_CMatrix tm; PIMM_FOR_WB(r, c) tm.m[r][c] = v; return tm;}
	static _CMatrix rotation(double angle) {return _CMatrix();}
	static _CMatrix rotationX(double angle) {return _CMatrix();}
	static _CMatrix rotationY(double angle) {return _CMatrix();}
	static _CMatrix rotationZ(double angle) {return _CMatrix();}
	static _CMatrix scaleX(double factor) {return _CMatrix();}
	static _CMatrix scaleY(double factor) {return _CMatrix();}
	static _CMatrix scaleZ(double factor) {return _CMatrix();}

	uint cols() const {return Cols;}
	uint rows() const {return Rows;}
	_CMCol col(uint index) {_CMCol tv; PIMM_FOR_R(i) tv[i] = m[i][index]; return tv;}
	_CMRow row(uint index) {_CMRow tv; PIMM_FOR_C(i) tv[i] = m[index][i]; return tv;}
	_CMatrix & setCol(uint index, const _CMCol & v) {PIMM_FOR_R(i) m[i][index] = v[i]; return *this;}
	_CMatrix & setRow(uint index, const _CMRow & v) {PIMM_FOR_C(i) m[index][i] = v[i]; return *this;}
	_CMatrix & swapRows(uint r0, uint r1) {Type t; PIMM_FOR_C(i) {t = m[r0][i]; m[r0][i] = m[r1][i]; m[r1][i] = t;} return *this;}
	_CMatrix & swapCols(uint c0, uint c1) {Type t; PIMM_FOR_R(i) {t = m[i][c0]; m[i][c0] = m[i][c1]; m[i][c1] = t;} return *this;}
	_CMatrix & fill(const Type & v) {PIMM_FOR_WB(r, c) m[r][c] = v; return *this;}
	//inline _CMatrix & set(Type fval, ...) {m[0] = fval; va_list vl; va_start(vl, fval); PIMV_FOR(i, 1) m[i] = va_arg(vl, Type); va_end(vl); return *this;}
	//inline void normalize() {Type tv = length(); if (tv == Type(1)) return; PIMV_FOR(i, 0) m[i] /= tv;}
	bool isSquare() const {return cols() == rows();}
	bool isIdentity() const {PIMM_FOR_WB(r, c) if ((c == r) ? m[r][c] != Type(1) : m[r][c] != Type(0)) return false; return true;}
	bool isNull() const {PIMM_FOR_WB(r, c) if (m[r][c] != Type(0)) return false; return true;}

	Type & at(uint row, uint col) {return m[row][col];}
	Type at(uint row, uint col) const {return m[row][col];}
	Type * operator [](uint row) {return m[row];}
	const Type * operator [](uint row) const {return m[row];}
	_CMatrix & operator =(const _CMatrix & sm) {memcpy(m, sm.m, sizeof(Type) * Cols * Rows); return *this;}
	bool operator ==(const _CMatrix & sm) const {PIMM_FOR_WB(r, c) if (m[r][c] != sm.m[r][c]) return false; return true;}
	bool operator !=(const _CMatrix & sm) const {return !(*this == sm);}
	void operator +=(const _CMatrix & sm) {PIMM_FOR_WB(r, c) m[r][c] += sm.m[r][c];}
	void operator -=(const _CMatrix & sm) {PIMM_FOR_WB(r, c) m[r][c] -= sm.m[r][c];}
	void operator *=(const Type & v) {PIMM_FOR_WB(r, c) m[r][c] *= v;}
	void operator /=(const Type & v) {PIMM_FOR_WB(r, c) m[r][c] /= v;}
	_CMatrix operator -() const {_CMatrix tm; PIMM_FOR_WB(r, c) tm.m[r][c] = -m[r][c]; return tm;}
	_CMatrix operator +(const _CMatrix & sm) const {_CMatrix tm = _CMatrix(*this); PIMM_FOR_WB(r, c) tm.m[r][c] += sm.m[r][c]; return tm;}
	_CMatrix operator -(const _CMatrix & sm) const {_CMatrix tm = _CMatrix(*this); PIMM_FOR_WB(r, c) tm.m[r][c] -= sm.m[r][c]; return tm;}
	_CMatrix operator *(const Type & v) const {_CMatrix tm = _CMatrix(*this); PIMM_FOR_WB(r, c) tm.m[r][c] *= v; return tm;}
	_CMatrix operator /(const Type & v) const {_CMatrix tm = _CMatrix(*this); PIMM_FOR_WB(r, c) tm.m[r][c] /= v; return tm;}
	
	Type determinant(bool * ok = 0) const {
		_CMatrix m(*this);
		bool k;
		Type ret = Type(0);
		m.toUpperTriangular(&k);
		if (ok) *ok = k;
		if (!k) return ret;
		ret = Type(1);
		for (uint c = 0; c < Cols; ++c)
			for (uint r = 0; r < Rows; ++r)
				if (r == c)
					ret *= m[r][c];
		return ret;
	}

	_CMatrix & toUpperTriangular(bool * ok = 0) {
		if (Cols != Rows) {
			if (ok != 0) *ok = false;
			return *this;
		}
		_CMatrix smat(*this);
		bool ndet;
		uint crow;
		Type mul;
		for (uint i = 0; i < Cols; ++i) {
			ndet = true;
			for (uint j = 0; j < Rows; ++j) if (smat.m[i][j] != 0) ndet = false;
			if (ndet) {
				if (ok != 0) *ok = false;
				return *this;
			}
			for (uint j = 0; j < Cols; ++j) if (smat.m[j][i] != 0) ndet = false;
			if (ndet) {
				if (ok != 0) *ok = false;
				return *this;
			}
		}
		for (uint i = 0; i < Cols; ++i) {
			crow = i;
			while (smat.m[i][i] == Type(0))
				smat.swapRows(i, ++crow);
			for (uint j = i + 1; j < Rows; ++j) {
				mul = smat.m[i][j] / smat.m[i][i];
				for (uint k = i; k < Cols; ++k) smat.m[k][j] -= mul * smat.m[k][i];
			}
			if (i < Cols - 1) {
				if (fabs(smat.m[i+1][i+1]) < Type(1E-100)) {
					if (ok != 0) *ok = false;
					return *this;
				}
			}
		}
		if (ok != 0) *ok = true;
		memcpy(m, smat.m, sizeof(Type) * Cols * Rows);
		return *this;
	}

	_CMatrix & invert(bool * ok = 0) {
		if (Cols != Rows) {
			if (ok != 0) *ok = false;
			return *this;
		}
		_CMatrix mtmp = _CMatrix::identity(), smat(*this);
		bool ndet;
		uint crow;
		Type mul, iddiv;
		for (uint i = 0; i < Cols; ++i) {
			ndet = true;
			for (uint j = 0; j < Rows; ++j) if (smat.m[i][j] != 0) ndet = false;
			if (ndet) {
				if (ok != 0) *ok = false;
				return *this;
			}
			for (uint j = 0; j < Cols; ++j) if (smat.m[j][i] != 0) ndet = false;
			if (ndet) {
				if (ok != 0) *ok = false;
				return *this;
			}
		}
		for (uint i = 0; i < Cols; ++i) {
			crow = i;
			while (smat.m[i][i] == Type(0)) {
				++crow;
				smat.swapRows(i, crow);
				mtmp.swapRows(i, crow);
			}
			for (uint j = i + 1; j < Rows; ++j) {
				mul = smat.m[i][j] / smat.m[i][i];
				for (uint k = i; k < Cols; ++k) smat.m[k][j] -= mul * smat.m[k][i];
				for (uint k = 0; k < Cols; ++k) mtmp.m[k][j] -= mul * mtmp.m[k][i];
			}
			//cout << i << endl << smat << endl;
			if (i < Cols - 1) {
				if (fabs(smat.m[i+1][i+1]) < Type(1E-100)) {
					if (ok != 0) *ok = false;
					return *this;
				}
			}
			iddiv = smat.m[i][i];
			for (uint j = i; j < Cols; ++j) smat.m[j][i] /= iddiv;
			for (uint j = 0; j < Cols; ++j) mtmp.m[j][i] /= iddiv;
		}
		for (uint i = Cols - 1; i > 0; --i) {
			for (uint j = 0; j < i; ++j) {
				mul = smat.m[i][j];
				smat.m[i][j] -= mul;
				for (uint k = 0; k < Cols; ++k) mtmp.m[k][j] -= mtmp.m[k][i] * mul;
			}
		}
		if (ok != 0) *ok = true;
		memcpy(m, mtmp.m, sizeof(Type) * Cols * Rows);
		return *this;
	}
	_CMatrix inverted(bool * ok = 0) const {_CMatrix tm(*this); tm.invert(ok); return tm;}
	_CMatrixI transposed() const {_CMatrixI tm; PIMM_FOR_WB(r, c) tm[c][r] = m[r][c]; return tm;}

private:
	void resize(uint rows_, uint cols_, const Type & new_value = Type()) {r_ = rows_; c_ = cols_; PIMM_FOR_WB(r, c) m[r][c] = new_value;}
	int c_, r_;
	Type m[Rows][Cols];

};
#pragma pack(pop)


template<> inline PIMathMatrixT<2u, 2u> PIMathMatrixT<2u, 2u>::rotation(double angle) {double c = cos(angle), s = sin(angle); PIMathMatrixT<2u, 2u> tm; tm[0][0] = tm[1][1] = c; tm[0][1] = -s; tm[1][0] = s; return tm;}
template<> inline PIMathMatrixT<2u, 2u> PIMathMatrixT<2u, 2u>::scaleX(double factor) {PIMathMatrixT<2u, 2u> tm; tm[0][0] = factor; tm[1][1] = 1.; return tm;}
template<> inline PIMathMatrixT<2u, 2u> PIMathMatrixT<2u, 2u>::scaleY(double factor) {PIMathMatrixT<2u, 2u> tm; tm[0][0] = 1.; tm[1][1] = factor; return tm;}

template<> inline PIMathMatrixT<3u, 3u> PIMathMatrixT<3u, 3u>::rotationX(double angle) {double c = cos(angle), s = sin(angle); PIMathMatrixT<3u, 3u> tm; tm[0][0] = 1.; tm[1][1] = tm[2][2] = c; tm[2][1] = s; tm[1][2] = -s; return tm;}
template<> inline PIMathMatrixT<3u, 3u> PIMathMatrixT<3u, 3u>::rotationY(double angle) {double c = cos(angle), s = sin(angle); PIMathMatrixT<3u, 3u> tm; tm[1][1] = 1.; tm[0][0] = tm[2][2] = c; tm[2][0] = -s; tm[0][2] = s; return tm;}
template<> inline PIMathMatrixT<3u, 3u> PIMathMatrixT<3u, 3u>::rotationZ(double angle) {double c = cos(angle), s = sin(angle); PIMathMatrixT<3u, 3u> tm; tm[2][2] = 1.; tm[0][0] = tm[1][1] = c; tm[1][0] = s; tm[0][1] = -s; return tm;}
template<> inline PIMathMatrixT<3u, 3u> PIMathMatrixT<3u, 3u>::scaleX(double factor) {PIMathMatrixT<3u, 3u> tm; tm[1][1] = tm[2][2] = 1.; tm[0][0] = factor; return tm;}
template<> inline PIMathMatrixT<3u, 3u> PIMathMatrixT<3u, 3u>::scaleY(double factor) {PIMathMatrixT<3u, 3u> tm; tm[0][0] = tm[2][2] = 1.; tm[1][1] = factor; return tm;}
template<> inline PIMathMatrixT<3u, 3u> PIMathMatrixT<3u, 3u>::scaleZ(double factor) {PIMathMatrixT<3u, 3u> tm; tm[0][0] = tm[1][1] = 1.; tm[2][2] = factor; return tm;}

template<uint Rows, uint Cols, typename Type>
inline std::ostream & operator <<(std::ostream & s, const PIMathMatrixT<Rows, Cols, Type> & m) {s << '{'; PIMM_FOR_I(r, c) s << m[r][c]; if (c < Cols - 1 || r < Rows - 1) s << ", ";} if (r < Rows - 1) s << std::endl << ' ';} s << '}'; return s;}
template<uint Rows, uint Cols, typename Type>
inline PICout operator <<(PICout s, const PIMathMatrixT<Rows, Cols, Type> & m) {s << '{'; PIMM_FOR_I(r, c) s << m[r][c]; if (c < Cols - 1 || r < Rows - 1) s << ", ";} if (r < Rows - 1) s << PICoutManipulators::NewLine << ' ';} s << '}'; return s;}

/// Multiply matrices {Rows0 x CR} on {CR x Cols1}, result is {Rows0 x Cols1}
template<uint CR, uint Rows0, uint Cols1, typename Type>
inline PIMathMatrixT<Rows0, Cols1, Type> operator *(const PIMathMatrixT<Rows0, CR, Type> & fm,
													const PIMathMatrixT<CR, Cols1, Type> & sm) {
	PIMathMatrixT<Rows0, Cols1, Type> tm;
	Type t;
	for (uint j = 0; j < Rows0; ++j) {
		for (uint i = 0; i < Cols1; ++i) {
			t = Type(0);
			for (uint k = 0; k < CR; ++k)
				t += fm[j][k] * sm[k][i];
			tm[j][i] = t;
		}
	}
	return tm;
}

/// Multiply matrix {Rows x Cols} on vector {Cols}, result is vector {Rows}
template<uint Cols, uint Rows, typename Type>
inline PIMathVectorT<Rows, Type> operator *(const PIMathMatrixT<Rows, Cols, Type> & fm,
											const PIMathVectorT<Cols, Type> & sv) {
	PIMathVectorT<Rows, Type> tv;
	Type t;
	for (uint j = 0; j < Rows; ++j) {
		t = Type(0);
		for (uint i = 0; i < Cols; ++i)
			t += fm[j][i] * sv[i];
		tv[j] = t;
	}
	return tv;
}

/// Multiply vector {Rows} on matrix {Rows x Cols}, result is vector {Cols}
template<uint Cols, uint Rows, typename Type>
inline PIMathVectorT<Cols, Type> operator *(const PIMathVectorT<Rows, Type> & sv,
											const PIMathMatrixT<Rows, Cols, Type> & fm) {
	PIMathVectorT<Cols, Type> tv;
	Type t;
	for (uint j = 0; j < Cols; ++j) {
		t = Type(0);
		for (uint i = 0; i < Rows; ++i)
			t += fm[i][j] * sv[i];
		tv[j] = t;
	}
	return tv;
}

/// Multiply value(T) on matrix {Rows x Cols}, result is vector {Rows}
template<uint Cols, uint Rows, typename Type>
inline PIMathMatrixT<Rows, Cols, Type> operator *(const Type & x, const PIMathMatrixT<Rows, Cols, Type> & v) {
	return v * x;
}


typedef PIMathMatrixT<2u, 2u, int> PIMathMatrixT22i;
typedef PIMathMatrixT<3u, 3u, int> PIMathMatrixT33i;
typedef PIMathMatrixT<4u, 4u, int> PIMathMatrixT44i;
typedef PIMathMatrixT<2u, 2u, double> PIMathMatrixT22d;
typedef PIMathMatrixT<3u, 3u, double> PIMathMatrixT33d;
typedef PIMathMatrixT<4u, 4u, double> PIMathMatrixT44d;


template<typename Type>
class PIMathMatrix;

#undef PIMV_FOR
#undef PIMM_FOR
#undef PIMM_FOR_WB
#undef PIMM_FOR_I
#undef PIMM_FOR_I_WB
#undef PIMM_FOR_C
#undef PIMM_FOR_R





/// Matrix

#define PIMM_FOR(c, r) for (uint c = 0; c < cols_; ++c) { for (uint r = 0; r < rows_; ++r) {
#define PIMM_FOR_WB(c, r) for (uint c = 0; c < cols_; ++c) for (uint r = 0; r < rows_; ++r) // without brakes
#define PIMM_FOR_I(c, r) for (uint r = 0; r < rows_; ++r) { for (uint c = 0; c < cols_; ++c) {
#define PIMM_FOR_I_WB(c, r) for (uint r = 0; r < rows_; ++r) for (uint c = 0; c < cols_; ++c) // without brakes
#define PIMM_FOR_C(v) for (uint v = 0; v < cols_; ++v)
#define PIMM_FOR_R(v) for (uint v = 0; v < rows_; ++v)

template<typename Type>
class PIP_EXPORT PIMathMatrix {
	typedef PIMathMatrix<Type> _CMatrix;
	typedef PIMathVector<Type> _CMCol;
	typedef PIMathVector<Type> _CMRow;
public:
	PIMathMatrix(const uint cols = 3, const uint rows = 3) {resize(cols, rows);}
	PIMathMatrix(const uint cols, const uint rows, Type fval, ...) {resize(cols, rows); va_list vl; va_start(vl, fval); PIMM_FOR_I_WB(c, r) m[c][r] = (r + c == 0 ? fval : va_arg(vl, Type)); va_end(vl);}
	PIMathMatrix(const uint cols, const uint rows, const PIVector<Type> & val) {resize(cols, rows); int i = 0; PIMM_FOR_I_WB(c, r) m[c][r] = val[i++];}

	static _CMatrix identity(const uint cols_, const uint rows_) {_CMatrix tm(cols_, rows_); PIMM_FOR_WB(c, r) tm.m[c][r] = (c == r ? Type(1) : Type(0)); return tm;}

	uint cols() const {return cols_;}
	uint rows() const {return rows_;}
	_CMCol col(uint index) {_CMCol tv; PIMM_FOR_R(i) tv[i] = m[index][i]; return tv;}
	_CMRow row(uint index) {_CMRow tv; PIMM_FOR_C(i) tv[i] = m[i][index]; return tv;}
	_CMatrix & resize(const uint cols, const uint rows, const Type & new_value = Type()) {cols_ = cols; rows_ = rows; m.resize(cols); PIMM_FOR_C(i) m[i].resize(rows, new_value); return *this;}
	_CMatrix & setCol(uint index, const _CMCol & v) {PIMM_FOR_R(i) m[index][i] = v[i]; return *this;}
	_CMatrix & setRow(uint index, const _CMRow & v) {PIMM_FOR_C(i) m[i][index] = v[i]; return *this;}
	_CMatrix & swapRows(uint r0, uint r1) {Type t; PIMM_FOR_C(i) {t = m[i][r0]; m[i][r0] = m[i][r1]; m[i][r1] = t;} return *this;}
	_CMatrix & swapCols(uint c0, uint c1) {Type t; PIMM_FOR_R(i) {t = m[c0][i]; m[c0][i] = m[c1][i]; m[c1][i] = t;} return *this;}
	_CMatrix & fill(const Type & v) {PIMM_FOR_WB(c, r) m[c][r] = v; return *this;}
	//inline _CMatrix & set(Type fval, ...) {m[0] = fval; va_list vl; va_start(vl, fval); PIMV_FOR(i, 1) m[i] = va_arg(vl, Type); va_end(vl); return *this;}
	//inline void normalize() {Type tv = length(); if (tv == Type(1)) return; PIMV_FOR(i, 0) m[i] /= tv;}
	bool isSquare() const {return cols() == rows();}
	bool isIdentity() const {PIMM_FOR_WB(c, r) if ((c == r) ? m[c][r] != Type(1) : m[c][r] != Type(0)) return false; return true;}
	bool isNull() const {PIMM_FOR_WB(c, r) if (m[c][r] != Type(0)) return false; return true;}

	Type & at(uint col, uint row) {return m[col][row];}
	Type at(uint col, uint row) const {return m[col][row];}
	PIVector<Type> & operator [](uint col) {return m[col];}
	PIVector<Type> operator [](uint col) const {return m[col];}
	void operator =(const _CMatrix & sm) {m = sm.m;}
	bool operator ==(const _CMatrix & sm) const {PIMM_FOR_WB(c, r) if (m[c][r] != sm.m[c][r]) return false; return true;}
	bool operator !=(const _CMatrix & sm) const {return !(*this == sm);}
	void operator +=(const _CMatrix & sm) {PIMM_FOR_WB(c, r) m[c][r] += sm.m[c][r];}
	void operator -=(const _CMatrix & sm) {PIMM_FOR_WB(c, r) m[c][r] -= sm.m[c][r];}
	void operator *=(const Type & v) {PIMM_FOR_WB(c, r) m[c][r] *= v;}
	void operator /=(const Type & v) {PIMM_FOR_WB(c, r) m[c][r] /= v;}
	_CMatrix operator -() {_CMatrix tm(*this); PIMM_FOR_WB(c, r) tm.m[c][r] = -m[c][r]; return tm;}
	_CMatrix operator +(const _CMatrix & sm) {_CMatrix tm(*this); PIMM_FOR_WB(c, r) tm.m[c][r] += sm.m[c][r]; return tm;}
	_CMatrix operator -(const _CMatrix & sm) {_CMatrix tm(*this); PIMM_FOR_WB(c, r) tm.m[c][r] -= sm.m[c][r]; return tm;}
	_CMatrix operator *(const Type & v) {_CMatrix tm(*this); PIMM_FOR_WB(c, r) tm.m[c][r] *= v; return tm;}
	_CMatrix operator /(const Type & v) {_CMatrix tm(*this); PIMM_FOR_WB(c, r) tm.m[c][r] /= v; return tm;}

	_CMatrix & toUpperTriangular(bool * ok = 0) {
		if (cols_ != rows_) {
			if (ok != 0) *ok = false;
			return *this;
		}
		_CMatrix smat(*this);
		bool ndet;
		uint crow;
		Type mul;
		for (uint i = 0; i < cols_; ++i) {
			ndet = true;
			for (uint j = 0; j < rows_; ++j) if (smat.m[i][j] != 0) ndet = false;
			if (ndet) {
				if (ok != 0) *ok = false;
				return *this;
			}
			for (uint j = 0; j < cols_; ++j) if (smat.m[j][i] != 0) ndet = false;
			if (ndet) {
				if (ok != 0) *ok = false;
				return *this;
			}
		}
		for (uint i = 0; i < cols_; ++i) {
			crow = i;
			while (smat.m[i][i] == Type(0))
				smat.swapRows(i, ++crow);
			for (uint j = i + 1; j < rows_; ++j) {
				mul = smat.m[i][j] / smat.m[i][i];
				for (uint k = i; k < cols_; ++k) smat.m[k][j] -= mul * smat.m[k][i];
			}
			if (i < cols_ - 1) {
				if (fabs(smat.m[i+1][i+1]) < Type(1E-100)) {
					if (ok != 0) *ok = false;
					return *this;
				}
			}
		}
		if (ok != 0) *ok = true;
		m = smat.m;
		return *this;
	}

	_CMatrix & invert(bool * ok = 0, _CMCol * sv = 0) {
		if (cols_ != rows_) {
			if (ok != 0) *ok = false;
			return *this;
		}
		_CMatrix mtmp = _CMatrix::identity(cols_, rows_), smat(*this);
		bool ndet;
		uint crow;
		Type mul, iddiv;
		for (uint i = 0; i < cols_; ++i) {
			ndet = true;
			for (uint j = 0; j < rows_; ++j) if (smat.m[i][j] != 0) ndet = false;
			if (ndet) {
				if (ok != 0) *ok = false;
				return *this;
			}
			for (uint j = 0; j < cols_; ++j) if (smat.m[j][i] != 0) ndet = false;
			if (ndet) {
				if (ok != 0) *ok = false;
				return *this;
			}
		}
		for (uint i = 0; i < cols_; ++i) {
			crow = i;
			while (smat.m[i][i] == Type(0)) {
				++crow;
				smat.swapRows(i, crow);
				mtmp.swapRows(i, crow);
				if (sv != 0) sv->swap(i, crow);
			}
			for (uint j = i + 1; j < rows_; ++j) {
				mul = smat.m[i][j] / smat.m[i][i];
				for (uint k = i; k < cols_; ++k) smat.m[k][j] -= mul * smat.m[k][i];
				for (uint k = 0; k < cols_; ++k) mtmp.m[k][j] -= mul * mtmp.m[k][i];
				if (sv != 0) (*sv)[j] -= mul * (*sv)[i];
			}
			//cout << i << endl << smat << endl;
			if (i < cols_ - 1) {
				if (fabs(smat.m[i+1][i+1]) < Type(1E-100)) {
					if (ok != 0) *ok = false;
					return *this;
				}
			}
			iddiv = smat.m[i][i];
			for (uint j = i; j < cols_; ++j) smat.m[j][i] /= iddiv;
			for (uint j = 0; j < cols_; ++j) mtmp.m[j][i] /= iddiv;
			if (sv != 0) (*sv)[i] /= iddiv;
		}
		for (uint i = cols_ - 1; i > 0; --i) {
			for (uint j = 0; j < i; ++j) {
				mul = smat.m[i][j];
				smat.m[i][j] -= mul;
				for (uint k = 0; k < cols_; ++k) mtmp.m[k][j] -= mtmp.m[k][i] * mul;
				if (sv != 0) (*sv)[j] -= mul * (*sv)[i];
			}
		}
		if (ok != 0) *ok = true;
		m = mtmp.m;
		return *this;
	}
	_CMatrix inverted(bool * ok = 0) {_CMatrix tm(*this); tm.invert(ok); return tm;}
	_CMatrix transposed() {_CMatrix tm(rows_, cols_); PIMM_FOR_WB(c, r) tm[r][c] = m[c][r]; return tm;}

private:
	uint cols_, rows_;
	PIVector<PIVector<Type> > m;

};

template<typename Type>
inline std::ostream & operator <<(std::ostream & s, const PIMathMatrix<Type> & m) {s << '{'; for (uint r = 0; r < m.rows(); ++r) { for (uint c = 0; c < m.cols(); ++c) { s << m[c][r]; if (c < m.cols() - 1 || r < m.rows() - 1) s << ", ";} if (r < m.rows() - 1) s << std::endl << ' ';} s << '}'; return s;}
template<typename Type>
inline PICout operator <<(PICout s, const PIMathMatrix<Type> & m) {s << '{'; for (uint r = 0; r < m.rows(); ++r) { for (uint c = 0; c < m.cols(); ++c) { s << m[c][r]; if (c < m.cols() - 1 || r < m.rows() - 1) s << ", ";} if (r < m.rows() - 1) s << PICoutManipulators::NewLine << ' ';} s << '}'; return s;}

/// Multiply matrices {CR x Rows0} on {Cols1 x CR}, result is {Cols1 x Rows0}
template<typename Type>
inline PIMathMatrix<Type> operator *(const PIMathMatrix<Type> & fm,
									const PIMathMatrix<Type> & sm) {
	uint cr = fm.cols(), rows0 = fm.rows(), cols1 = sm.cols();
	PIMathMatrix<Type> tm(cols1, rows0);
	if (fm.cols() != sm.rows()) return tm;
	Type t;
	for (uint j = 0; j < rows0; ++j) {
		for (uint i = 0; i < cols1; ++i) {
			t = Type(0);
			for (uint k = 0; k < cr; ++k)
				t += fm[k][j] * sm[i][k];
			tm[i][j] = t;
		}
	}
	return tm;
}

/// Multiply matrix {Cols x Rows} on vector {Cols}, result is vector {Rows}
template<typename Type>
inline PIMathVector<Type> operator *(const PIMathMatrix<Type> & fm,
									const PIMathVector<Type> & sv) {
	uint c = fm.cols(), r = fm.rows();
	PIMathVector<Type> tv(r);
	if (c != sv.size()) return tv;
	Type t;
	for (uint i = 0; i < r; ++i) {
		t = Type(0);
		for (uint j = 0; j < c; ++j)
			t += fm[j][i] * sv[j];
		tv[i] = t;
	}
	return tv;
}

typedef PIMathMatrix<int> PIMathMatrixi;
typedef PIMathMatrix<double> PIMathMatrixd;

#undef PIMV_FOR
#undef PIMM_FOR
#undef PIMM_FOR_WB
#undef PIMM_FOR_I
#undef PIMM_FOR_I_WB
#undef PIMM_FOR_C
#undef PIMM_FOR_R

#endif // PIMATHMATRIX_H
