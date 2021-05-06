/*! \file piquaternion.h
 * \brief Class for quaternions
*/
/*
	PIP - Platform Independent Primitives
	Class for quaternions
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

#ifndef PIQUATERNION_H
#define PIQUATERNION_H

#include "pimathmatrix.h"

class PIQuaternion
{
	friend PIQuaternion operator*(const PIQuaternion & q0, const PIQuaternion & q1);
	friend PIQuaternion operator*(const double & a, const PIQuaternion & q);
public:
	PIQuaternion(const PIMathVectorT3d & u = PIMathVectorT3d(), double a = 0.);
	
	PIQuaternion conjugate() const {return PIQuaternion(-vector(), scalar());}
	PIQuaternion rotated(const PIMathVectorT3d & u, double a) const;
	void rotate(const PIMathVectorT3d & u, double a);
	void normalize();
	
	double & scalar() {return q[0];}
	double scalar() const {return q[0];}
	PIMathVectorT3d vector() const {return PIMathVectorT3d(q[1], q[2], q[3]);}
	
	PIMathVectorT3d eyler() const;
	PIMathMatrixT33d rotationMatrix() const;
	void axis(PIMathVectorT3d*ret) const;
	
	static PIQuaternion fromEyler(double ax, double ay, double az);
	static PIQuaternion fromRotationMatrix(const PIMathMatrixT33d & m);
	static PIQuaternion fromAngles(double ax, double ay, double az);
	static PIQuaternion fromAngles2(double ax, double ay, double az);

protected:
	double q[4];
	PIMathMatrixT44d makeMatrix() const;
	
	
};

PIQuaternion operator *(const double & a, const PIQuaternion & q);
//PIQuaternion operator *(const PIQuaternion & q, const double & a) {return a*q;}
PIQuaternion operator *(const PIQuaternion & q0, const PIQuaternion & q1);
inline PIQuaternion operator +(const PIQuaternion & q0, const PIQuaternion & q1) {return PIQuaternion(q0.vector() + q1.vector(), q0.scalar() + q1.scalar());}
inline PIQuaternion operator -(const PIQuaternion & q0, const PIQuaternion & q1) {return PIQuaternion(q0.vector() - q1.vector(), q0.scalar() - q1.scalar());}
inline PIQuaternion operator -(const PIQuaternion & q0) {return PIQuaternion(-q0.vector(), -q0.scalar());}

#endif // PIQUATERNION_H
