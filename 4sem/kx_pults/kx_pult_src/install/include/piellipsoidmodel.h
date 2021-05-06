/*! \file piellipsoidmodel.h
 * \brief Contains geo ellipsoid models
*/
/*
    PIP - Platform Independent Primitives
    Contains geo ellipsoid models
	Copyright (C) 2016 Andrey Bychkov work.a.b@yandex.ru

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

#ifndef PIELLIPSOIDMODEL_H
#define PIELLIPSOIDMODEL_H


#include "pimathbase.h"

class PIEllipsoidModel {
public:
	PIEllipsoidModel();
	double eccSquared() const {return eccentricity * eccentricity;} // eccentricity squared
	double b() const {return a * sqrt(1 - eccSquared());}

	static PIEllipsoidModel WGS84Ellipsoid();
	static PIEllipsoidModel PZ90Ellipsoid();
	static PIEllipsoidModel GPSEllipsoid();

	double a; /// Major axis of Earth in meters
	double flattening; /// Flattening (ellipsoid parameter)
	double eccentricity; /// Eccentricity (ellipsoid parameter)
	double angVelocity; /// Angular velocity of Earth in radians/sec
};



#endif // PIELLIPSOIDMODEL_H
