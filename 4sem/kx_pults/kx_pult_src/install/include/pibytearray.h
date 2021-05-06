/*! \file pibytearray.h
 * \brief Byte array
*/
/*
	PIP - Platform Independent Primitives
	Byte array
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

#ifndef PIBYTEARRAY_H
#define PIBYTEARRAY_H

#ifdef DOXYGEN
//! This macro allow stream template operators for write and read any type from byte array. Use it with attention!
#  define PIP_BYTEARRAY_STREAM_ANY_TYPE
#endif

#include "pibitarray.h"

class PIString;
class PIByteArray;


class PIP_EXPORT PIByteArray: public PIDeque<uchar>
{
public:

	//! Constructs an empty byte array
	PIByteArray() {;}

	//! Constructs 0-filled byte array with size "size"
	PIByteArray(const uint size) {resize(size);}

	//! Constructs byte array from data "data" and size "size"
	PIByteArray(const void * data, const uint size): PIDeque<uchar>((const uchar*)data, size_t(size)) {/*for (uint i = 0; i < size; ++i) push_back(((uchar * )data)[i]);*/}


	//! Help struct to store/restore custom blocks of data to/from PIByteArray
	struct RawData {
		friend PIByteArray & operator <<(PIByteArray & s, const PIByteArray::RawData & v);
		friend PIByteArray & operator >>(PIByteArray & s, PIByteArray::RawData v);
	public:
		//! Constructs data block
		RawData(void * data = 0, int size = 0) {d = data; s = size;}
		RawData(const RawData & o) {d = o.d; s = o.s;}
		//! Constructs data block
		RawData(const void * data, const int size) {d = const_cast<void * >(data); s = size;}
		RawData & operator =(const RawData & o) {d = o.d; s = o.s; return *this;}
	private:
		void * d;
		int s;
	};

	//! Return resized byte array
	PIByteArray resized(int new_size) const {PIByteArray tv(*this); tv.resize(new_size); return tv;}

	//! Convert data to Base 64 and return this byte array
	PIByteArray & convertToBase64();

	//! Convert data from Base 64 and return this byte array
	PIByteArray & convertFromBase64();

	//! Return converted to Base 64 data
	PIByteArray toBase64() const;

	//! Return converted from Base 64 data

	PIByteArray & compressRLE(uchar threshold = 192);
	PIByteArray & decompressRLE(uchar threshold = 192);
	PIByteArray compressedRLE(uchar threshold = 192) {PIByteArray ba(*this); ba.compressRLE(threshold); return ba;}
	PIByteArray decompressedRLE(uchar threshold = 192) {PIByteArray ba(*this); ba.decompressRLE(threshold); return ba;}

	PIString toString(int base = 16) const;
	PIString toHex() const;

	//! Add to the end data "data" with size "size"
	PIByteArray & append(const void * data_, int size_) {uint ps = size(); enlarge(size_); memcpy(data(ps), data_, size_); return *this;}

	//! Add to the end byte array "data"
	PIByteArray & append(const PIByteArray & data_) {uint ps = size(); enlarge(data_.size_s()); memcpy(data(ps), data_.data(), data_.size()); return *this;}
	/*PIByteArray & operator <<(short v) {for (uint i = 0; i < sizeof(v); ++i) push_back(((uchar*)(&v))[i]); return *this;}
	PIByteArray & operator <<(ushort v) {for (uint i = 0; i < sizeof(v); ++i) push_back(((uchar*)(&v))[i]); return *this;}
	PIByteArray & operator <<(int v) {for (uint i = 0; i < sizeof(v); ++i) push_back(((uchar*)(&v))[i]); return *this;}
	PIByteArray & operator <<(uint v) {for (uint i = 0; i < sizeof(v); ++i) push_back(((uchar*)(&v))[i]); return *this;}
	PIByteArray & operator <<(llong v) {for (uint i = 0; i < sizeof(v); ++i) push_back(((uchar*)(&v))[i]); return *this;}
	PIByteArray & operator <<(ullong v) {for (uint i = 0; i < sizeof(v); ++i) push_back(((uchar*)(&v))[i]); return *this;}*/
	//PIByteArray & operator <<(const PIByteArray & v) {for (uint i = 0; i < v.size(); ++i) push_back(v[i]); return *this;}

	//! Returns plain 8-bit checksum
	uchar checksumPlain8() const;

	//! Returns plain 32-bit checksum
	uint checksumPlain32() const;

	void operator =(const PIDeque<uchar> & d) {resize(d.size()); memcpy(data(), d.data(), d.size());}

	static PIByteArray fromString(PIString str);
	static PIByteArray fromHex(PIString str);
	static PIByteArray fromBase64(const PIByteArray & base64);
	static PIByteArray fromBase64(const PIString & base64);
};

inline bool operator <(const PIByteArray & v0, const PIByteArray & v1) {if (v0.size() == v1.size()) {for (uint i = 0; i < v0.size(); ++i) if (v0[i] != v1[i]) return v0[i] < v1[i]; return false;} return v0.size() < v1.size();}
//! \relatesalso PIByteArray \brief Output to std::ostream operator
inline std::ostream & operator <<(std::ostream & s, const PIByteArray & ba) {s << "{"; for (uint i = 0; i < ba.size(); ++i) {s << ba[i]; if (i < ba.size() - 1) s << ", ";} s << "}"; return s;}

//! \relatesalso PIByteArray \brief Output to PICout operator
inline PICout operator <<(PICout s, const PIByteArray & ba) {s.space(); s.setControl(0, true); s << "{"; for (uint i = 0; i < ba.size(); ++i) {s << ba[i]; if (i < ba.size() - 1) s << ", ";} s << "}"; s.restoreControl(); return s;}

#define PBA_OPERATOR_TO int os = s.size_s(); s.enlarge(sizeof(v)); memcpy(s.data(os), &v, sizeof(v));

//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, bool v) {s.push_back(v); return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, char v) {s.push_back(v); return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, uchar v) {s.push_back(v); return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, const short v) {PBA_OPERATOR_TO return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, const int v) {PBA_OPERATOR_TO return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, const long & v) {PBA_OPERATOR_TO return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, const llong & v) {PBA_OPERATOR_TO return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, const ushort v) {PBA_OPERATOR_TO return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, const uint v) {PBA_OPERATOR_TO return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, const ulong & v) {PBA_OPERATOR_TO return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, const ullong & v) {PBA_OPERATOR_TO return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, const float v) {PBA_OPERATOR_TO return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, const double & v) {PBA_OPERATOR_TO return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, const ldouble & v) {PBA_OPERATOR_TO return s;}
//! \relatesalso PIByteArray \brief Store operator, see \ref PIByteArray_sec1 for details
inline PIByteArray & operator <<(PIByteArray & s, const PIByteArray & v) {s << int(v.size_s()); int os = s.size_s(); s.enlarge(v.size_s()); if (v.size_s() > 0) memcpy(s.data(os), v.data(), v.size()); return s;}
//! \relatesalso PIByteArray \brief Store operator, see \ref PIByteArray_sec1 for details
inline PIByteArray & operator <<(PIByteArray & s, const PIByteArray::RawData & v) {int os = s.size_s(); s.enlarge(v.s); if (v.s > 0) memcpy(s.data(os), v.d, v.s); return s;}
//! \relatesalso PIByteArray \brief Store operator
template<typename Type0, typename Type1>
inline PIByteArray & operator <<(PIByteArray & s, const PIPair<Type0, Type1> & v) {s << v.first << v.second; return s;}
//! \relatesalso PIByteArray \brief Store operator
template<typename T>
inline PIByteArray & operator <<(PIByteArray & s, const PIVector<T> & v) {s << int(v.size_s()); for (uint i = 0; i < v.size(); ++i) s << v[i]; return s;}
//! \relatesalso PIByteArray \brief Store operator
template<typename T>
inline PIByteArray & operator <<(PIByteArray & s, const PIList<T> & v) {s << int(v.size_s()); for (uint i = 0; i < v.size(); ++i) s << v[i]; return s;}
//! \relatesalso PIByteArray \brief Store operator
inline PIByteArray & operator <<(PIByteArray & s, const PIBitArray & v) {s << v.size_ << v.data_; return s;}
//! \relatesalso PIByteArray \brief Store operator
template<typename T>
inline PIByteArray & operator <<(PIByteArray & s, const PIDeque<T> & v) {s << int(v.size_s()); for (uint i = 0; i < v.size(); ++i) s << v[i]; return s;}
#ifdef PIP_BYTEARRAY_STREAM_ANY_TYPE
template<typename T>
inline PIByteArray & operator <<(PIByteArray & s, const T & v) {PBA_OPERATOR_TO return s;}
#endif

#undef PBA_OPERATOR_TO
#define PBA_OPERATOR_FROM memcpy(&v, s.data(), sizeof(v)); s.remove(0, sizeof(v));

//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, bool & v) {assert(s.size() >= 1u); v = s.take_front(); return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, char & v) {assert(s.size() >= 1u); v = s.take_front(); return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, uchar & v) {assert(s.size() >= 1u); v = s.take_front(); return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, short & v) {assert(s.size() >= sizeof(v)); PBA_OPERATOR_FROM return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, int & v) {assert(s.size() >= sizeof(v)); PBA_OPERATOR_FROM return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, long & v) {assert(s.size() >= sizeof(v)); PBA_OPERATOR_FROM return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, llong & v) {assert(s.size() >= sizeof(v)); PBA_OPERATOR_FROM return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, ushort & v) {assert(s.size() >= sizeof(v)); PBA_OPERATOR_FROM return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, uint & v) {assert(s.size() >= sizeof(v)); PBA_OPERATOR_FROM return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, ulong & v) {assert(s.size() >= sizeof(v)); PBA_OPERATOR_FROM return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, ullong & v) {assert(s.size() >= sizeof(v)); PBA_OPERATOR_FROM return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, float & v) {assert(s.size() >= sizeof(v)); PBA_OPERATOR_FROM return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, double & v) {assert(s.size() >= sizeof(v)); PBA_OPERATOR_FROM return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, ldouble & v) {assert(s.size() >= sizeof(v)); PBA_OPERATOR_FROM return s;}
//! \relatesalso PIByteArray \brief Restore operator, see \ref PIByteArray_sec1 for details
inline PIByteArray & operator >>(PIByteArray & s, PIByteArray & v) {assert(s.size_s() >= 4); int sz; s >> sz; v.resize(sz); if (sz > 0) memcpy(v.data(), s.data(), v.size()); s.remove(0, v.size()); return s;}
//! \relatesalso PIByteArray \brief Restore operator, see \ref PIByteArray_sec1 for details
inline PIByteArray & operator >>(PIByteArray & s, PIByteArray::RawData v) {assert(s.size_s() >= v.s); if (v.s > 0) memcpy(v.d, s.data(), v.s); s.remove(0, v.s); return s;}
//! \relatesalso PIByteArray \brief Restore operator
template<typename Type0, typename Type1>
inline PIByteArray & operator >>(PIByteArray & s, PIPair<Type0, Type1> & v) {s >> v.first >> v.second; return s;}
//! \relatesalso PIByteArray \brief Restore operator
template<typename T>
inline PIByteArray & operator >>(PIByteArray & s, PIVector<T> & v) {assert(s.size_s() >= 4); int sz; s >> sz; v.resize(sz); for (int i = 0; i < sz; ++i) s >> v[i]; return s;}
//! \relatesalso PIByteArray \brief Restore operator
template<typename T>
inline PIByteArray & operator >>(PIByteArray & s, PIList<T> & v) {assert(s.size_s() >= 4); int sz; s >> sz; v.resize(sz); for (int i = 0; i < sz; ++i) s >> v[i]; return s;}
//! \relatesalso PIByteArray \brief Restore operator
template<typename T>
inline PIByteArray & operator >>(PIByteArray & s, PIDeque<T> & v) {assert(s.size_s() >= 4); int sz; s >> sz; v.resize(sz); for (int i = 0; i < sz; ++i) s >> v[i]; return s;}
//! \relatesalso PIByteArray \brief Restore operator
inline PIByteArray & operator >>(PIByteArray & s, PIBitArray & v) {assert(s.size_s() >= 8); s >> v.size_ >> v.data_; return s;}
// //! \relatesalso PIByteArray \brief Restore operator
//template <typename Key, typename T>
//inline PIByteArray & operator >>(PIByteArray & s, PIMap<Key, T> & v) {assert(s.size_s() >= 4); int sz; s >> sz; v.resize(sz); for (int i = 0; i < sz; ++i) s >> v[i]; return s;}
#ifdef PIP_BYTEARRAY_STREAM_ANY_TYPE
template<typename T>
inline PIByteArray & operator >>(PIByteArray & s, T & v) {assert(s.size() >= sizeof(v)); PBA_OPERATOR_FROM return s;}
#endif


#undef PBA_OPERATOR_FROM

//! \relatesalso PIByteArray \brief Byte arrays compare operator
inline bool operator ==(PIByteArray & f, PIByteArray & s) {if (f.size_s() != s.size_s()) return false; for (int i = 0; i < f.size_s(); ++i) if (f[i] != s[i]) return false; return true;}
//! \relatesalso PIByteArray \brief Byte arrays compare operator
inline bool operator !=(PIByteArray & f, PIByteArray & s) {if (f.size_s() != s.size_s()) return true; for (int i = 0; i < f.size_s(); ++i) if (f[i] != s[i]) return true; return false;}

#endif // PIBYTEARRAY_H
