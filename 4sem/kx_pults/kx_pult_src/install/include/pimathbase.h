/*! \file pimathbase.h
 * \brief Basic mathematical functions and defines
*/
/*
	PIP - Platform Independent Primitives
	Basic mathematical functions and defines
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

#ifndef PIMATHBASE_H
#define PIMATHBASE_H

#include "piinit.h"
#include "pibytearray.h"
#ifdef QNX
#  undef PIP_MATH_J0
#  undef PIP_MATH_J1
#  undef PIP_MATH_JN
#  undef PIP_MATH_Y0
#  undef PIP_MATH_Y1
#  undef PIP_MATH_YN
#  include <math.h>
#  ifdef BLACKBERRY
#    include <complex>
#  else
#    include <complex>
#  endif
#else
#  include <complex>
#  include <cmath>
#endif

#ifndef M_LN2
#  define M_LN2 0.69314718055994530942
#endif
#ifndef M_LN10
#  define M_LN10 2.30258509299404568402
#endif
#ifndef M_SQRT2
#  define M_SQRT2 1.41421356237309514547
#endif
#ifndef M_SQRT3
#  define M_SQRT3 1.73205080756887719318
#endif
#ifndef M_1_SQRT2
#  define M_1_SQRT2 0.70710678118654746172
#endif
#ifndef M_1_SQRT3
#  define M_1_SQRT3 0.57735026918962584208
#endif
#ifndef M_PI
#  define M_PI 3.141592653589793238462643383280
#endif
#ifndef M_2PI
#  define M_2PI 6.283185307179586476925286766559
#endif
#ifndef M_PI_3
#  define M_PI_3 1.04719755119659774615
#endif
#ifndef M_2PI_3
#  define M_2PI_3 2.0943951023931954923
#endif
#ifndef M_180_PI
#  define M_180_PI 57.2957795130823208768
#endif
#ifndef M_PI_180
#  define M_PI_180 1.74532925199432957692e-2
#endif
#ifndef M_SQRT_PI
#  define M_SQRT_PI 1.772453850905516027298167483341
#endif
#ifndef M_E
#  define M_E 2.7182818284590452353602874713527
#endif
#ifndef M_LIGHT_SPEED
#  define M_LIGHT_SPEED 2.99792458e+8
#endif
#ifndef M_RELATIVE_CONST
#  define M_RELATIVE_CONST -4.442807633e-10;
#endif
#ifndef M_GRAVITY_CONST
#  define M_GRAVITY_CONST 398600.4418e9;
#endif

using std::complex;

typedef complex<int> complexi;
typedef complex<float> complexf;
typedef complex<double> complexd;
typedef complex<ldouble> complexld;

const complexld complexld_i(0., 1.);
const complexld complexld_0(0.);
const complexld complexld_1(1.);
const complexd complexd_i(0., 1.);
const complexd complexd_0(0.);
const complexd complexd_1(1.);

__PICONTAINERS_SIMPLE_TYPE__(complexi)
__PICONTAINERS_SIMPLE_TYPE__(complexf)
__PICONTAINERS_SIMPLE_TYPE__(complexd)
__PICONTAINERS_SIMPLE_TYPE__(complexld)

const double deg2rad = M_PI_180;
const double rad2deg = M_180_PI;

inline int sign(const float & x) {return (x < 0.) ? -1 : (x > 0. ? 1 : 0);}
inline int sign(const double & x) {return (x < 0.) ? -1 : (x > 0. ? 1 : 0);}
inline complexd sign(const complexd & x) {return complexd(sign(x.real()), sign(x.imag()));}
inline int pow2(const int p) {return 1 << p;}
inline double sqr(const int v) {return v * v;}
inline double sqr(const float & v) {return v * v;}
inline double sqr(const double & v) {return v * v;}
inline double sinc(const double & v) {if (v == 0.) return 1.; double t = M_PI * v; return sin(t) / t;}
inline complexd round(const complexd & c) {return complexd(piRound<double>(c.real()), piRound<double>(c.imag()));}
inline complexd floor(const complexd & c) {return complexd(floor(c.real()), floor(c.imag()));}
inline complexd ceil(const complexd & c) {return complexd(ceil(c.real()), ceil(c.imag()));}
inline complexd atanc(const complexd & c) {return -complexd(-0.5, 1.) * log((complexd_1 + complexd_i * c) / (complexd_1 - complexd_i * c));}
inline complexd asinc(const complexd & c) {return -complexd_i * log(complexd_i * c + sqrt(complexd_1 - c * c));}
inline complexd acosc(const complexd & c) {return -complexd_i * log(c + complexd_i * sqrt(complexd_1 - c * c));}
#ifdef CC_GCC
#  if CC_GCC_VERSION <= 0x025F
inline complexd tan(const complexd & c) {return sin(c) / cos(c);}
inline complexd tanh(const complexd & c) {return sinh(c) / cosh(c);}
inline complexd log2(const complexd & c) {return log(c) / M_LN2;}
inline complexd log10(const complexd & c) {return log(c) / M_LN10;}
#  endif
#endif
double piJ0(const double & v);
double piJ1(const double & v);
double piJn(int n, const double & v);
double piY0(const double & v);
double piY1(const double & v);
double piYn(int n, const double & v);
inline double toDb(double val) {return 10. * log10(val);}
inline double fromDb(double val) {return pow(10., val / 10.);}
inline double toRad(double deg) {return deg * M_PI_180;}
inline double toDeg(double rad) {return rad * M_180_PI;}

template<typename T>
inline PICout operator <<(PICout s, const complex<T> & v) {s.space(); s.setControl(0, true); s << "(" << v.real() << "; " << v.imag() << ")"; s.restoreControl(); return s;}

//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, complexf v) {float t; t = v.real(); s << t; t = v.imag(); s << t; return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, complexd v) {double t; t = v.real(); s << t; t = v.imag(); s << t; return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, complexld v) {ldouble t; t = v.real(); s << t; t = v.imag(); s << t; return s;}

//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, complexf & v) {float t0, t1; s >> t0; s >> t1; v = complexf(t0, t1); return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, complexd & v) {double t0, t1; s >> t0; s >> t1; v = complexd(t0, t1); return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, complexld & v) {ldouble t0, t1; s >> t0; s >> t1; v = complexld(t0, t1); return s;}

void randomize();

// [-1 ; 1]
inline double randomd() {return (double)
#ifdef QNX
										rand()
#else
										random()
#endif
												/ RAND_MAX * 2. - 1.;}
// [-1 ; 1] normal
double randomn(double dv = 0., double sv = 1.);

inline PIVector<double> abs(const PIVector<complexd> & v) {
	PIVector<double> result;
	result.resize(v.size());
	for (uint i = 0; i < v.size(); i++)
		result[i] = abs(v[i]);
	return result;
}
inline PIVector<double> abs(const PIVector<double> & v) {
	PIVector<double> result;
	result.resize(v.size());
	for (uint i = 0; i < v.size(); i++)
        result[i] = fabs(v[i]);
	return result;
}


template <typename T>
bool OLS_Linear(const PIVector<PIPair<T, T> > & input, T * out_a, T * out_b) {
	if (input.size_s() < 2)
		return false;
	int n = input.size_s();
	T a_t0 = T(), a_t1 = T(), a_t2 = T(), a_t3 = T(), a_t4 = T(), a = T(), b = T();
	for (int i = 0; i < n; ++i) {
		const PIPair<T, T> & cv(input[i]);
		a_t0 += cv.first * cv.second;
		a_t1 += cv.first;
		a_t2 += cv.second;
		a_t3 += cv.first * cv.first;
	}
	a_t4 = n * a_t3 - a_t1 * a_t1;
	if (a_t4 != T())
		a = (n * a_t0 - a_t1 * a_t2) / a_t4;
	b = (a_t2 - a * a_t1) / n;
	if (out_a != 0) *out_a = a;
	if (out_b != 0) *out_b = b;
	return true;
}


template <typename T>
bool WLS_Linear(const PIVector<PIPair<T, T> > & input, const PIVector<T> & weights, T * out_a, T * out_b) {
	if (input.size_s() < 2)
		return false;
	if (input.size_s() != weights.size_s())
		return false;
	int n = input.size_s();
	T a_t0 = T(), a_t1 = T(), a_t2 = T(), a_t3 = T(), a_t4 = T(), a_n = T(), a = T(), b = T();
	for (int i = 0; i < n; ++i) {
		T cp = weights[i];
		const PIPair<T, T> & cv(input[i]);
		a_t0 += cv.first * cv.second * cp;
		a_t1 += cv.first * cp;
		a_t2 += cv.second * cp;
		a_t3 += cv.first * cv.first * cp;
		a_n += cp;
	}
	a_t4 = a_n * a_t3 - a_t1 * a_t1;
	if (a_t4 != T())
		a = (a_n * a_t0 - a_t1 * a_t2) / a_t4;
	b = (a_t2 - a * a_t1) / a_n;
	if (out_a != 0) *out_a = a;
	if (out_b != 0) *out_b = b;
	return true;
}

#endif // PIMATHBASE_H
