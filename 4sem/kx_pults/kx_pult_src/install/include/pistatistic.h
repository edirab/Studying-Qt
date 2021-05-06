/*! \file pistatistic.h
 * \brief Class for calculating math statistic in values array
*/
/*
	PIP - Platform Independent Primitives
	Class for calculacing math statistic in values array
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

#ifndef PISTATISTIC_H
#define PISTATISTIC_H

#include "pimathbase.h"

template <typename T>
class PIP_EXPORT PIStatistic {
public:
	PIStatistic() {mean = variance = skewness = kurtosis = T();}
	
	static T calculateMean(const PIVector<T> & val) {
		T ret = T();
		int n = val.size();
		if (n < 1)
			return ret;
		for (int i = 0; i < n; i++)
			ret += val[i];
		return ret / n;
	}
	bool calculate(const PIVector<T> & val, const T & given_mean) {
		T v = T(), v1 = T(), v2 = T(), stddev = T(), var = T();
		int i, n = val.size();
		if (n < 2)
			return false;
		mean = given_mean;
		variance = skewness = kurtosis = T();
		/*
		* Variance (using corrected two-pass algorithm)
		*/
		for (i = 0; i < n; i++)
			v1 += sqr(val[i] - mean);
		for (i = 0; i < n; i++)
			v2 += val[i] - mean;
		v2 = sqr(v2) / n;
		variance = v1 / n;
		var = (v1 / n - v2) / (n - 1);
		if (var < T())
			var = T();
		stddev = sqrt(var);
		/*
		* Skewness and kurtosis
		*/
		if (stddev != T()) {
			for (i = 0; i < n; i++) {
				v = (val[i] - mean) / stddev;
				v2 = sqr(v);
				skewness = skewness + v2 * v;
				kurtosis = kurtosis + sqr(v2);
			}
			skewness /= n;
			kurtosis = kurtosis / n - 3.;
		}
		return true;
	}
	bool calculate(const PIVector<T> & val) {return calculate(val, calculateMean(val));}
	
	T mean;
	T variance;
	T skewness;
	T kurtosis;
};

typedef PIStatistic<int> PIStatistici;
typedef PIStatistic<float> PIStatisticf;
typedef PIStatistic<double> PIStatisticd;

#endif // PISTATISTIC_H
