/*
    PIP - Platform Independent Primitives
    Bit array
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

#ifndef PIBITARRAY_H
#define PIBITARRAY_H

#include "picontainers.h"

class PIP_EXPORT PIBitArray {
	friend PIByteArray & operator <<(PIByteArray & s, const PIBitArray & v);
	friend PIByteArray & operator >>(PIByteArray & s, PIBitArray & v);
public:
	PIBitArray(const int & size = 0) {resize(size);}
	PIBitArray(uchar val) {resize(sizeof(val) * 8); data_[0] = val;}
	PIBitArray(ushort val) {resize(sizeof(val) * 8); memcpy(data(), &val, sizeof(val));}
	PIBitArray(uint val) {resize(sizeof(val) * 8); memcpy(data(), &val, sizeof(val));}
	PIBitArray(ulong val) {resize(sizeof(val) * 8); memcpy(data(), &val, sizeof(val));}
	PIBitArray(ullong val) {resize(sizeof(val) * 8); memcpy(data(), &val, sizeof(val));}
	PIBitArray(uchar * bytes, uint size) {resize(size * 8); memcpy(data(), bytes, size);}

	uint bitSize() const {return size_;}
	uint byteSize() const {return bytesInBits(size_);}
	PIBitArray & resize(const uint & size) {size_ = size; data_.resize(bytesInBits(size_)); return *this;}

	PIBitArray & clearBit(const uint & index) {data_[index / 8] &= ~(1 << (index % 8)); return *this;}
	PIBitArray & setBit(const uint & index) {data_[index / 8] |= (1 << (index % 8)); return *this;}
	PIBitArray & writeBit(const uint & index, const bool & value) {if (value) setBit(index); else clearBit(index); return *this;}
	PIBitArray & writeBit(const uint & index, const uchar & value) {return writeBit(index, value > 0);}

	PIBitArray & push_back(const bool & value) {resize(size_ + 1); writeBit(size_ - 1, value); return *this;}
	PIBitArray & push_back(const uchar & value) {return push_back(value > 0);}
	PIBitArray & insert(const uint & index, const bool & value) {
		resize(size_ + 1);
		uint fi = byteSize() - 1, si = index / 8, ti = index % 8;
		uchar c = data_[si];
		for (uint i = fi; i > si; --i) {
			data_[i] <<= 1;
			if ((0x80 & data_[i - 1]) == 0x80) data_[i] |= 1;
			else data_[i] &= 0xFE;}
		data_[si] &= (0xFF >> (7 - ti));
		data_[si] |= ((c << 1) & (0xFF << (ti)));
		if (value) data_[si] |= (1 << ti);
		else data_[si] &= ~(1 << ti);
		return *this;}
	PIBitArray & insert(const uint & index, const uchar & value) {return insert(index, value > 0);}
	PIBitArray & push_front(const bool & value) {return insert(0, value);}
	PIBitArray & push_front(const uchar & value) {return push_front(value > 0);}
	PIBitArray & pop_back() {return resize(size_ - 1);}
	PIBitArray & pop_front() {
		if (size_ == 0) return *this;
		uint fi = byteSize() - 1;
		for (uint i = 0; i < fi; ++i) {
			data_[i] >>= 1;
			if ((1 & data_[i + 1]) == 1) data_[i] |= 0x80;
			else data_[i] &= 0x7F;}
		data_[fi] >>= 1;
		resize(size_ - 1);
		return *this;}
	PIBitArray & append(const PIBitArray & ba) {for (uint i = 0; i < ba.bitSize(); ++i) push_back(ba[i]); return *this;}

	uchar * data() {return data_.data();}
	uchar toUChar() {if (size_ == 0) return 0; return data_[0];}
	ushort toUShort() {ushort t = 0; memcpy(&t, data(), piMin<uint>(byteSize(), sizeof(t))); return t;}
	uint toUInt() {uint t = 0; memcpy(&t, data(), piMin<uint>(byteSize(), sizeof(t))); return t;}
	ulong toULong() {ulong t = 0; memcpy(&t, data(), piMin<uint>(byteSize(), sizeof(t))); return t;}
	ullong toULLong() {ullong t = 0; memcpy(&t, data(), piMin<uint>(byteSize(), sizeof(t))); return t;}

	bool at(const uint & index) const {return (1 & (data_[index / 8] >> (index % 8))) == 1 ? true : false;}
	bool operator [](const uint & index) const {return at(index);}
	void operator +=(const PIBitArray & ba) {append(ba);}
	bool operator ==(const PIBitArray & ba) const {if (bitSize() != ba.bitSize()) return false; for (uint i = 0; i < bitSize(); ++i) if (at(i) != ba[i]) return false; return true;}
	bool operator !=(const PIBitArray & ba) const {return !(*this == ba);}
	void operator =(const uchar & val) {resize(sizeof(val) * 8); data_[0] = val;}
	void operator =(const ushort & val) {resize(sizeof(val) * 8); memcpy(data(), &val, sizeof(val));}
	void operator =(const uint & val) {resize(sizeof(val) * 8); memcpy(data(), &val, sizeof(val));}
	void operator =(const ulong & val) {resize(sizeof(val) * 8); memcpy(data(), &val, sizeof(val));}
	void operator =(const ullong & val) {resize(sizeof(val) * 8); memcpy(data(), &val, sizeof(val));}

private:
	static uint bytesInBits(const uint & bits) {return (bits + 7) / 8;}

	PIVector<uchar> data_;
	uint size_;

};

inline std::ostream & operator <<(std::ostream & s, const PIBitArray & ba) {for (uint i = 0; i < ba.bitSize(); ++i) {s << ba[i]; if (i % 8 == 7) s << ' ';} return s;}
inline PICout operator <<(PICout s, const PIBitArray & ba) {s.space(); s.setControl(0, true); for (uint i = 0; i < ba.bitSize(); ++i) {s << int(ba[i]); if (i % 8 == 7) s << ' ';} s.restoreControl(); return s;}

#endif // PIBITARRAY_H
