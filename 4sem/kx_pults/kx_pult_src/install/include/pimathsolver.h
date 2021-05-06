/*! \file pimathsolver.h
 * \brief PIMathSolver
*/
/*
	PIP - Platform Independent Primitives
	PIMathSolver
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

#ifndef PIMATHSOLVER_H
#define PIMATHSOLVER_H

#include "pimathmatrix.h"

/// Differential evaluations

struct TransferFunction {
	PIVector<double> vector_Bm, vector_An;
};

class PIP_EXPORT PIMathSolver
{
public:
	enum Method {Global = -1,
				Eyler_1 = 01,
				Eyler_2 = 02,
				EylerKoshi = 03,
				RungeKutta_4 = 14,
				AdamsBashfortMoulton_2 = 22,
				AdamsBashfortMoulton_3 = 23,
				AdamsBashfortMoulton_4 = 24,
				PolynomialApproximation_2 = 32,
				PolynomialApproximation_3 = 33,
				PolynomialApproximation_4 = 34,
				PolynomialApproximation_5 = 35
				};

	PIMathSolver() {times.resize(4); step = 0;}

	void solve(double u, double h);
	void fromTF(const TransferFunction & TF);
	void setMethod(Method m) {method = m;}
	void setTime(double time) {times.pop_back(); times.push_front(time);}

	void solveEyler1(double u, double h);
	void solveEyler2(double u, double h);
	void solveRK4(double u, double h);
	void solveABM2(double u, double h);
	void solveABM3(double u, double h);
	void solveABM4(double u, double h);
	void solvePA(double u, double h, uint deg);
	void solvePA2(double u, double h) {if (step > 0) solvePA(u, h, 2); else solveEyler1(u, h);}
	void solvePA3(double u, double h) {if (step > 1) solvePA(u, h, 3); else solvePA2(u, h);}
	void solvePA4(double u, double h) {if (step > 2) solvePA(u, h, 4); else solvePA3(u, h);}
	void solvePA5(double u, double h) {if (step > 3) solvePA(u, h, 5); else solvePA4(u, h);}

	PIMathVectord X;
	static Method method_global;
	static const char methods_desc[];

private:
	void moveF() {for (uint i = F.size() - 1; i > 0; --i) F[i] = F[i - 1];}

	PIMathMatrixd A, M;
	PIMathVectord d, a1, b1;
	PIMathVectord k1, k2, k3, k4, xx;
	PIMathVectord XX, Y, pY;
	PIVector<PIMathVectord> F;
	PIVector<double> times;
	uint size, step;
	Method method;
	double sum, td, ct, lp, dh, t, x1, x0;
	bool ok;
};

#endif // PIMATHSOLVER_H
