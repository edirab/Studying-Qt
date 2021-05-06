/*! \file pifft.h
 * \brief Class for FFT, IFFT and Hilbert transformations
*/
/*
	PIP - Platform Independent Primitives
	Class for FFT, IFFT and Hilbert transformations
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

#ifndef PIFFT_H
#define PIFFT_H

#include "pimathbase.h"

class PIP_EXPORT PIFFT_double
{
public:
	PIFFT_double();
	
	PIVector<complexd> * calcFFT(const PIVector<complexd> &val);
	PIVector<complexd> * calcFFT(const PIVector<double> &val);
	PIVector<complexd> * calcFFTinverse(const PIVector<complexd> &val);
	PIVector<complexd> * calcHilbert(const PIVector<double> &val);
	PIVector<double> getAmplitude();
	
private:
	PIVector<complexd> result;
	typedef ptrdiff_t ae_int_t;

	struct ftplan {
		PIVector<int> plan;
		PIVector<double> precomputed;
		PIVector<double> tmpbuf;
		PIVector<double> stackbuf;
	};

	ftplan curplan;

	void fftc1d(const PIVector<complexd> &a, uint n);
	void fftc1r(const PIVector<double> &a, uint n);
	void fftc1dinv(const PIVector<complexd> &a, uint n);

	void createPlan(uint n);
	void ftbasegeneratecomplexfftplan(uint n, ftplan *plan);
	void ftbase_ftbasegenerateplanrec(int n, int tasktype, ftplan *plan, int *plansize, int *precomputedsize, int *planarraysize, int *tmpmemsize, int *stackmemsize, ae_int_t stackptr, int debugi=0);
	void ftbase_ftbaseprecomputeplanrec(ftplan *plan, int entryoffset, ae_int_t stackptr);
	void ftbasefactorize(int n, int *n1, int *n2);
	void ftbase_ftbasefindsmoothrec(int n, int seed, int leastfactor, int *best);
	int ftbasefindsmooth(int n);
	void ftbaseexecuteplan(PIVector<double> *a, int aoffset, int n, ftplan *plan);
	void ftbaseexecuteplanrec(PIVector<double> *a, int aoffset, ftplan *plan, int entryoffset, ae_int_t stackptr);
	void ftbase_internalcomplexlintranspose(PIVector<double> *a, int m, int n, int astart, PIVector<double> *buf);
	void ftbase_ffticltrec(PIVector<double> *a, int astart, int astride, PIVector<double> *b, int bstart, int bstride, int m, int n);
	void ftbase_internalreallintranspose(PIVector<double> *a, int m, int n, int astart, PIVector<double> *buf);
	void ftbase_fftirltrec(PIVector<double> *a, int astart, int astride, PIVector<double> *b, int bstart, int bstride, int m, int n);
	void ftbase_ffttwcalc(PIVector<double> *a, int aoffset, int n1, int n2);
};

class PIP_EXPORT PIFFT_float
{
public:
	PIFFT_float();

	PIVector<complexf> * calcFFT(const PIVector<complexf> &val);
	PIVector<complexf> * calcFFT(const PIVector<float> &val);
	PIVector<complexf> * calcFFTinverse(const PIVector<complexf> &val);
	PIVector<complexf> * calcHilbert(const PIVector<float> &val);
	PIVector<float> getAmplitude();

private:
	PIVector<complexf> result;
	typedef ptrdiff_t ae_int_t;

	struct ftplan {
		PIVector<int> plan;
		PIVector<float> precomputed;
		PIVector<float> tmpbuf;
		PIVector<float> stackbuf;
	};

	ftplan curplan;

	void fftc1d(const PIVector<complexf> &a, uint n);
	void fftc1r(const PIVector<float> &a, uint n);
	void fftc1dinv(const PIVector<complexf> &a, uint n);

	void createPlan(uint n);
	void ftbasegeneratecomplexfftplan(uint n, ftplan *plan);
	void ftbase_ftbasegenerateplanrec(int n, int tasktype, ftplan *plan, int *plansize, int *precomputedsize, int *planarraysize, int *tmpmemsize, int *stackmemsize, ae_int_t stackptr, int debugi=0);
	void ftbase_ftbaseprecomputeplanrec(ftplan *plan, int entryoffset, ae_int_t stackptr);
	void ftbasefactorize(int n, int *n1, int *n2);
	void ftbase_ftbasefindsmoothrec(int n, int seed, int leastfactor, int *best);
	int ftbasefindsmooth(int n);
	void ftbaseexecuteplan(PIVector<float> *a, int aoffset, int n, ftplan *plan);
	void ftbaseexecuteplanrec(PIVector<float> *a, int aoffset, ftplan *plan, int entryoffset, ae_int_t stackptr);
	void ftbase_internalcomplexlintranspose(PIVector<float> *a, int m, int n, int astart, PIVector<float> *buf);
	void ftbase_ffticltrec(PIVector<float> *a, int astart, int astride, PIVector<float> *b, int bstart, int bstride, int m, int n);
	void ftbase_internalreallintranspose(PIVector<float> *a, int m, int n, int astart, PIVector<float> *buf);
	void ftbase_fftirltrec(PIVector<float> *a, int astart, int astride, PIVector<float> *b, int bstart, int bstride, int m, int n);
	void ftbase_ffttwcalc(PIVector<float> *a, int aoffset, int n1, int n2);
};

typedef PIFFT_double PIFFT;
typedef PIFFT_double PIFFTd;
typedef PIFFT_float PIFFTf;

#endif // PIFFT_H
