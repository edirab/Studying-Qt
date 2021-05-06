/*! \file pigeoposition.h
 * \brief Class for geo position storage and conversions
*/
/*
	PIP - Platform Independent Primitives
	Class for geo position storage and conversions
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

#ifndef PIGEOPOSITION_H
#define PIGEOPOSITION_H

#include "piellipsoidmodel.h"
#include "pimathvector.h"

class PIGeoPosition : public PIMathVectorT3d
{
public:

	enum CoordinateSystem
	{
		Unknown=0,  /// Unknown coordinate system
		Geodetic,   /// Geodetic latitude, longitude, and height above ellipsoid
		Geocentric, /// Geocentric (regular spherical coordinates)
		Cartesian,  /// Cartesian (Earth-centered, Earth-fixed)
		Spherical   /// Spherical coordinates (theta,phi,radius)
	};

	static const double one_cm_tolerance;/// One centimeter tolerance.
	static const double one_mm_tolerance;/// One millimeter tolerance.
	static const double one_um_tolerance;/// One micron tolerance.
	static double position_tolerance;/// Default tolerance (default 1mm)
	static double setPositionTolerance(const double tol) {position_tolerance = tol; return position_tolerance;}
	static double getPositionTolerance() {return position_tolerance;}

	PIGeoPosition();
	PIGeoPosition(double a, double b, double c, CoordinateSystem s = Cartesian, PIEllipsoidModel ell = PIEllipsoidModel::WGS84Ellipsoid());
	PIGeoPosition(PIMathVectorT3d v, CoordinateSystem s = Cartesian, PIEllipsoidModel ell = PIEllipsoidModel::WGS84Ellipsoid());


	PIGeoPosition &transformTo(CoordinateSystem sys);
	PIGeoPosition &asGeodetic() {transformTo(Geodetic); return *this; } /// Convert to geodetic coordinate
	PIGeoPosition &asGeodetic(const PIEllipsoidModel &ell) {setEllipsoidModel(ell); transformTo(Geodetic); return *this;} /// Convert to another ell, then to geodetic coordinates
	PIGeoPosition &asECEF() {transformTo(Cartesian); return *this; } /// Convert to cartesian coordinates

	double x() const;
	double y() const;
	double z() const;
	double latitudeGeodetic() const;
	double latitudeGeocentric() const;
	double longitude() const;
	double theta() const;
	double phi() const;
	double radius() const;
	double height() const;

	/// Set the ellipsoid values for this PIGeoPosition given a ellipsoid.
	void setEllipsoidModel(const PIEllipsoidModel &ell) {el = ell;}

	/// Set the \a PIGeoPosition given geodetic coordinates in degrees. \a CoordinateSystem is set to \a Geodetic.
	PIGeoPosition &setGeodetic(double lat, double lon, double ht, PIEllipsoidModel ell = PIEllipsoidModel::WGS84Ellipsoid());

	/// Set the \a PIGeoPosition given geocentric coordinates in degrees. \a CoordinateSystem is set to \a Geocentric
	PIGeoPosition &setGeocentric(double lat, double lon, double rad);

	/// Set the \a PIGeoPosition given spherical coordinates in degrees. \a CoordinateSystem is set to \a Spherical
	PIGeoPosition &setSpherical(double theta, double phi, double rad);

	/// Set the \a PIGeoPosition given ECEF coordinates in meeters. \a CoordinateSystem is set to \a Cartesian.
	PIGeoPosition &setECEF(double x, double y, double z);

	/// Fundamental conversion from spherical to cartesian coordinates.
	static void convertSphericalToCartesian(const PIMathVectorT3d &tpr, PIMathVectorT3d &xyz);

	/// Fundamental routine to convert cartesian to spherical coordinates.
	static void convertCartesianToSpherical(const PIMathVectorT3d &xyz, PIMathVectorT3d &tpr);

	/// Fundamental routine to convert ECEF (cartesian) to geodetic coordinates,
	static void convertCartesianToGeodetic(const PIMathVectorT3d &xyz, PIMathVectorT3d &llh, PIEllipsoidModel ell = PIEllipsoidModel::WGS84Ellipsoid());

	/// Fundamental routine to convert geodetic to ECEF (cartesian) coordinates,
	static void convertGeodeticToCartesian(const PIMathVectorT3d &llh, PIMathVectorT3d &xyz, PIEllipsoidModel ell = PIEllipsoidModel::WGS84Ellipsoid());

	/// Fundamental routine to convert cartesian (ECEF) to geocentric
	static void convertCartesianToGeocentric(const PIMathVectorT3d &xyz, PIMathVectorT3d &llr);

	/// Fundamental routine to convert geocentric to cartesian (ECEF)
	static void convertGeocentricToCartesian(const PIMathVectorT3d &llr, PIMathVectorT3d &xyz);

	/// Fundamental routine to convert geocentric to geodetic
	static void convertGeocentricToGeodetic(const PIMathVectorT3d &llr, PIMathVectorT3d &llh, PIEllipsoidModel ell = PIEllipsoidModel::WGS84Ellipsoid());

	/// Fundamental routine to convert geodetic to geocentric
	static void convertGeodeticToGeocentric(const PIMathVectorT3d &llh, PIMathVectorT3d &llr, PIEllipsoidModel ell = PIEllipsoidModel::WGS84Ellipsoid());

	/// Compute the radius of the ellipsoidal Earth, given the geodetic latitude.
	static double radiusEarth(double geolat, PIEllipsoidModel ell = PIEllipsoidModel::WGS84Ellipsoid());
	double radiusEarth() const {
		PIGeoPosition p(*this);
		p.transformTo(PIGeoPosition::Geodetic);
		return PIGeoPosition::radiusEarth((*this)[0], p.el);
	}

	/// Compute the range in meters between two PIGeoPositions.
	static double range(const PIGeoPosition &a, const PIGeoPosition &b);
	double range(const PIGeoPosition &p) const {
		return range((*this), p);
	}

	/// Computes the elevation of the input (p) position as seen from this PIGeoPosition.
	double elevation(const PIGeoPosition &p) const;

	/// Computes the elevation of the input (p) position as seen from this PIGeoPosition, using a Geodetic (ellipsoidal) system.
	double elevationGeodetic(const PIGeoPosition &p) const;

	/// Computes the azimuth of the input (p) position as seen from this PIGeoPosition.
	double azimuth(const PIGeoPosition &p) const;

	/// Computes the azimuth of the input (p) position as seen from this PIGeoPosition, using a Geodetic (ellipsoidal) system.
	double azimuthGeodetic(const PIGeoPosition &p) const;

	/// Computes the radius of curvature of the meridian (Rm) corresponding to this PIGeoPosition.
	double getCurvMeridian() const;

	/// Computes the radius of curvature in the prime vertical (Rn) corresponding to this PIGeoPosition.
	double getCurvPrimeVertical() const;

	/// Returns as PIMathVectorT3d
	const PIMathVectorT3d & vector() const {return *this;}

//	PIGeoPosition &operator=(const PIGeoPosition & v);
	PIGeoPosition &operator=(const PIMathVectorT3d & v);
	PIGeoPosition &operator-=(const PIGeoPosition &right);
	PIGeoPosition &operator+=(const PIGeoPosition &right);
	friend PIGeoPosition operator-(const PIGeoPosition &left, const PIGeoPosition &right);
	friend PIGeoPosition operator+(const PIGeoPosition &left, const PIGeoPosition &right);
	friend PIGeoPosition operator*(const double &scale, const PIGeoPosition &right);
	friend PIGeoPosition operator*(const PIGeoPosition &left, const double &scale);
	friend PIGeoPosition operator*(const int &scale, const PIGeoPosition &right);
	friend PIGeoPosition operator*(const PIGeoPosition &left, const int &scale);
	bool operator==(const PIGeoPosition &right) const;
	bool operator!=(const PIGeoPosition &right) const {return !(operator==(right));}


private:
	void initialize(PIMathVectorT3d v, CoordinateSystem sys = Cartesian, PIEllipsoidModel ell = PIEllipsoidModel::WGS84Ellipsoid());

	PIEllipsoidModel el;
	CoordinateSystem s;

};


inline PIGeoPosition operator-(const PIGeoPosition &left, const PIGeoPosition &right) {PIGeoPosition l(left),r(right); l.transformTo(PIGeoPosition::Cartesian); r.transformTo(PIGeoPosition::Cartesian); l -= r; return l;}
inline PIGeoPosition operator+(const PIGeoPosition &left, const PIGeoPosition &right) {PIGeoPosition l(left),r(right); l.transformTo(PIGeoPosition::Cartesian); r.transformTo(PIGeoPosition::Cartesian); l += r; return l;}
inline PIGeoPosition operator*(const double &scale, const PIGeoPosition &right) {PIMathVectorT3d tmp(right); tmp *= scale; return PIGeoPosition(tmp);}
inline PIGeoPosition operator*(const PIGeoPosition &left, const double &scale) {return operator* (scale, left);}
inline PIGeoPosition operator*(const int &scale, const PIGeoPosition &right) {return operator* (double(scale), right);}
inline PIGeoPosition operator*(const PIGeoPosition &left, const int &scale) {return operator* (double(scale), left);}

#endif // PIGEOPOSITION_H
