/*! \file picrc.h
 * \brief CRC checksum calculator
*/
/*
    PIP - Platform Independent Primitives
    CRC checksum calculator
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

#ifndef PICRC_H
#define PICRC_H

#include "pistring.h"

template <int L>
class PIP_EXPORT uint_cl {
public:
	uint_cl() {for (int i = 0; i < L / 8; ++i) data_[i] = 0;}
	uint_cl(const uint_cl<L> & v) {for (int i = 0; i < L / 8; ++i) data_[i] = v.data_[i];}
	uint_cl(uchar v) {for (int i = 0; i < L / 8; ++i) data_[i] = (i == 0 ? v : 0);}
	uint_cl(char v) {for (int i = 0; i < L / 8; ++i) data_[i] = (i == 0 ? v : 0);}
	uint_cl(ushort v) {int l = piMin<uint>(L / 8, sizeof(v)); memcpy(data_, &v, l); for (int i = l; i < L / 8; ++i) data_[i] = 0;}
	uint_cl(short v) {int l = piMin<uint>(L / 8, sizeof(v)); memcpy(data_, &v, l); for (int i = l; i < L / 8; ++i) data_[i] = 0;}
	uint_cl(uint v) {int l = piMin<uint>(L / 8, sizeof(v)); memcpy(data_, &v, l); for (int i = l; i < L / 8; ++i) data_[i] = 0;}
	uint_cl(int v) {int l = piMin<uint>(L / 8, sizeof(v)); memcpy(data_, &v, l); for (int i = l; i < L / 8; ++i) data_[i] = 0;}
	uint_cl(ulong v) {int l = piMin<uint>(L / 8, sizeof(v)); memcpy(data_, &v, l); for (int i = l; i < L / 8; ++i) data_[i] = 0;}
	uint_cl(long v) {int l = piMin<uint>(L / 8, sizeof(v)); memcpy(data_, &v, l); for (int i = l; i < L / 8; ++i) data_[i] = 0;}
	uint_cl(ullong v) {int l = piMin<uint>(L / 8, sizeof(v)); memcpy(data_, &v, l); for (int i = l; i < L / 8; ++i) data_[i] = 0;}
	uint_cl(llong v) {int l = piMin<uint>(L / 8, sizeof(v)); memcpy(data_, &v, l); for (int i = l; i < L / 8; ++i) data_[i] = 0;}

	operator bool() {for (int i = 0; i < L / 8; ++i) if (data_[i] > 0) return true; return false;}
	operator char() {return (char)data_[0];}
	operator short() {short t(0); int l = piMin<uint>(L / 8, sizeof(t)); memcpy(&t, data_, l); return t;}
	operator int() {int t(0); int l = piMin<uint>(L / 8, sizeof(t)); memcpy(&t, data_, l); return t;}
	operator long() {long t(0); int l = piMin<uint>(L / 8, sizeof(t)); memcpy(&t, data_, l); return t;}
	operator llong() {llong t(0); int l = piMin<uint>(L / 8, sizeof(t)); memcpy(&t, data_, l); return t;}
	operator uchar() {return data_[0];}
	operator ushort() {ushort t(0); int l = piMin<uint>(L / 8, sizeof(t)); memcpy(&t, data_, l); return t;}
	operator uint() {uint t(0); int l = piMin<uint>(L / 8, sizeof(t)); memcpy(&t, data_, l); return t;}
	operator ulong() {ulong t(0); int l = piMin<uint>(L / 8, sizeof(t)); memcpy(&t, data_, l); return t;}
	operator ullong() {ullong t(0); int l = piMin<uint>(L / 8, sizeof(t)); memcpy(&t, data_, l); return t;}

	uint_cl<L> operator +(const uint_cl<L> & v) {
		uint_cl<L> t;
		uint cv;
		bool ov = false;
		for (int i = 0; i < L / 8; ++i) {
			cv = v.data_[i] + data_[i];
			if (ov) ++cv;
			ov = cv > 255;
			t.data_[i] = ov ? cv - 256 : cv;
		}
		return t;
	}

	uint_cl<L> operator &(const uint_cl<L> & v) const {uint_cl<L> t; for (int i = 0; i < L / 8; ++i) t.data_[i] = v.data_[i] & data_[i]; return t;}
	uint_cl<L> operator &(const uchar & v) const {return *this & uint_cl<L>(v);}
	uint_cl<L> operator &(const ushort & v) const {return *this & uint_cl<L>(v);}
	uint_cl<L> operator &(const uint & v) const {return *this & uint_cl<L>(v);}
	uint_cl<L> operator &(const ulong & v) const {return *this & uint_cl<L>(v);}
	uint_cl<L> operator &(const ullong & v) const {return *this & uint_cl<L>(v);}
	uint_cl<L> operator &(const char & v) const {return *this & uint_cl<L>(v);}
	uint_cl<L> operator &(const short & v) const {return *this & uint_cl<L>(v);}
	uint_cl<L> operator &(const int & v) const {return *this & uint_cl<L>(v);}
	uint_cl<L> operator &(const long & v) const {return *this & uint_cl<L>(v);}
	uint_cl<L> operator &(const llong & v) const {return *this & uint_cl<L>(v);}

	uint_cl<L> operator |(const uint_cl<L> & v) const {uint_cl<L> t; for (int i = 0; i < L / 8; ++i) t.data_[i] = v.data_[i] | data_[i]; return t;}
	uint_cl<L> operator |(const uchar & v) const {return *this | uint_cl<L>(v);}
	uint_cl<L> operator |(const ushort & v) const {return *this | uint_cl<L>(v);}
	uint_cl<L> operator |(const uint & v) const {return *this | uint_cl<L>(v);}
	uint_cl<L> operator |(const ulong & v) const {return *this | uint_cl<L>(v);}
	uint_cl<L> operator |(const ullong & v) const {return *this | uint_cl<L>(v);}
	uint_cl<L> operator |(const char & v) const {return *this | uint_cl<L>(v);}
	uint_cl<L> operator |(const short & v) const {return *this | uint_cl<L>(v);}
	uint_cl<L> operator |(const int & v) const {return *this | uint_cl<L>(v);}
	uint_cl<L> operator |(const long & v) const {return *this | uint_cl<L>(v);}
	uint_cl<L> operator |(const llong & v) const {return *this | uint_cl<L>(v);}

	uint_cl<L> operator ^(const uint_cl<L> & v) const {uint_cl<L> t; for (int i = 0; i < L / 8; ++i) t.data_[i] = v.data_[i] ^ data_[i]; return t;}
	uint_cl<L> operator ^(const uchar & v) const {return *this ^ uint_cl<L>(v);}
	uint_cl<L> operator ^(const ushort & v) const {return *this ^ uint_cl<L>(v);}
	uint_cl<L> operator ^(const uint & v) const {return *this ^ uint_cl<L>(v);}
	uint_cl<L> operator ^(const ulong & v) const {return *this ^ uint_cl<L>(v);}
	uint_cl<L> operator ^(const ullong & v) const {return *this ^ uint_cl<L>(v);}
	uint_cl<L> operator ^(const char & v) const {return *this ^ uint_cl<L>(v);}
	uint_cl<L> operator ^(const short & v) const {return *this ^ uint_cl<L>(v);}
	uint_cl<L> operator ^(const int & v) const {return *this ^ uint_cl<L>(v);}
	uint_cl<L> operator ^(const long & v) const {return *this ^ uint_cl<L>(v);}
	uint_cl<L> operator ^(const llong & v) const {return *this ^ uint_cl<L>(v);}

	bool operator <(const uint_cl<L> & v) const {for (int i = 0; i < L / 8; ++i) {if (v.data_[i] > data_[i]) return true; if (v.data_[i] < data_[i]) return false;} return false;}
	bool operator <=(const uint_cl<L> & v) const {for (int i = 0; i < L / 8; ++i) {if (v.data_[i] > data_[i]) return true; if (v.data_[i] < data_[i]) return false;} return true;}
	bool operator >(const uint_cl<L> & v) const {for (int i = 0; i < L / 8; ++i) {if (v.data_[i] < data_[i]) return true; if (v.data_[i] > data_[i]) return false;} return false;}
	bool operator >=(const uint_cl<L> & v) const {for (int i = 0; i < L / 8; ++i) {if (v.data_[i] < data_[i]) return true; if (v.data_[i] > data_[i]) return false;} return true;}
	bool operator ==(const uint_cl<L> & v) const {for (int i = 0; i < L / 8; ++i) if (v.data_[i] != data_[i]) return false; return true;}
	bool operator !=(const uint_cl<L> & v) const {for (int i = 0; i < L / 8; ++i) if (v.data_[i] != data_[i]) return true; return false;}
	bool operator <=(const uint_cl<8> & v1) {return (*(uchar*)data()) <= (*(uchar*)v1.data());}

	uint_cl<L> operator >>(const int & c) const {
		uint_cl<L> t;
		int l = L - c;
		bool bit;
		if (l <= 0) return t;
		for (int i = 0; i < l; ++i) {
			bit = 1 & (data_[(i + c) / 8] >> ((i + c) % 8));
			if (bit) t.data_[i / 8] |= (1 << (i % 8));
			else t.data_[i / 8] &= ~(1 << (i % 8));
		}
		return t;
	}
	uint_cl<L> operator >>(const uint & c) const {return (*this << (int)c);}
	uint_cl<L> operator <<(const int & c) const {
		uint_cl<L> t;
		int l = L - c;
		bool bit;
		if (l <= 0) return t;
		for (int i = c; i < L; ++i) {
			bit = 1 & (data_[(i - c) / 8] >> ((i - c) % 8));
			if (bit) t.data_[i / 8] |= (1 << (i % 8));
			else t.data_[i / 8] &= ~(1 << (i % 8));
		}
		return t;
	}
	uint_cl<L> operator <<(const uint & c) const {return (*this >> (int)c);}

	uint_cl<L> & inverse() const {for (int i = 0; i < L / 8; ++i) data_[i] = ~data_[i]; return *this;}
	uint_cl<L> inversed() const {uint_cl<L> t(*this); for (int i = 0; i < L / 8; ++i) t.data_[i] = ~t.data_[i]; return t;}
	uint_cl<L> reversed() const {
		uint_cl<L> t;
		bool bit;
		for (int i = 0; i < L; ++i) {
			bit = 1 & (data_[(L - i - 1) / 8] >> ((L - i - 1) % 8));
			if (bit) t.data_[i / 8] |= (1 << (i % 8));
			else t.data_[i / 8] &= ~(1 << (i % 8));
		}
		return t;
	}

	const uchar * data() const {return data_;}
	uchar * data() {return data_;}
	uint length() const {return L / 8;}

private:
	uchar data_[L / 8];

};

template <uint L>
inline std::ostream & operator <<(std::ostream & s, const uint_cl<L> & v) {std::ios::fmtflags f = s.flags(); s << std::hex; for (uint i = 0; i < v.length(); ++i) {s << int(v.data()[i]); if (v.data()[i] < 0x10) s << '0'; s << ' ';} s.flags(f); return s;}

inline uchar reverseByte(uchar b) {
	uchar ret = 0;
	bool bit;
	for (int i = 0; i < 8; ++i) {
		bit = 1 & (b >> (7 - i));
		if (bit) ret |= (1 << i);
	}
	return ret;
}

template <uint L, typename N = uint_cl<L> >
class PIP_EXPORT PICRC {
public:
	PICRC(const N & poly = N()) {poly_ = poly; reverse_poly = true; init_ = inversed(N(0)); out_ = inversed(N(0)); reverse_before_xor = reverse_data = false; initTable();}
	PICRC(const N & poly, bool reverse_poly_, const N & initial, const N & out_xor) {poly_ = poly; reverse_poly = reverse_poly_; init_ = initial; out_ = out_xor; reverse_before_xor = reverse_data = false; initTable();}

	void setInitial(const N & v) {init_ = v;}
	void setOutXor(const N & v) {out_ = v;}
	void setReversePolynome(bool yes) {reverse_poly = yes; initTable();}
	void setReverseOutBeforeXOR(bool yes) {reverse_before_xor = yes;}
	void setReverseDataBytes(bool yes) {reverse_data = yes;}

	void initTable() {
		N tmp, pol = reverse_poly ? reversed(poly_) : poly_;
		//cout << std::hex << "poly " << (uint)N(poly_) << " -> " << (uint)N(pol) << endl;
		for (int i = 0; i < 256; ++i) {
			tmp = uchar(i);
			for (int j = 0; j < 8; ++j)
				tmp = ((tmp & 1) ? ((tmp >> 1) ^ pol) : (tmp >> 1));
			table[i] = tmp;
		}

	}

	N calculate(const void * data, int size) {
		N crc = init_;
		uchar * data_ = (uchar * )data, cb;
		//cout << "process " << size << endl;
		uchar nTemp;
		for (int i = 0; i < size; ++i) {
			cb = data_[i];
			if (reverse_data) cb = reverseByte(cb);
			nTemp = cb ^ uchar(crc);
			crc = crc >> 8;
			crc = crc ^ table[nTemp];
		}
		if (reverse_before_xor) crc = reversed(crc);
		return crc ^ out_;

	}
	N calculate(const PIByteArray & d) {return calculate(d.data(), d.size());}
	N calculate(const char * str) {PIByteArray s(PIString(str).toByteArray()); return calculate(s.data(), s.size_s());}

private:
	inline N reversed(const N & v) {return v.reversed();}
	inline N inversed(const N & v) {return v.inversed();}
	
	N table[256];
	N poly_, init_, out_;
	bool reverse_poly, reverse_before_xor, reverse_data;

};

template <> inline uchar  PICRC<8, uchar>::reversed(const uchar & v) {return reverseByte(v);}
template <> inline ushort PICRC<16, ushort>::reversed(const ushort & v) {return uint_cl<16>(v).reversed();}
template <> inline uint   PICRC<32, uint>::reversed(const uint & v) {return uint_cl<32>(v).reversed();}
template <> inline uchar  PICRC<8, uchar>::inversed(const uchar & v) {return ~v;}
template <> inline ushort PICRC<16, ushort>::inversed(const ushort & v) {return ~v;}
template <> inline uint   PICRC<32, uint>::inversed(const uint & v) {return ~v;}

typedef PICRC<32, uint>   CRC_32;
typedef PICRC<24>         CRC_24;
typedef PICRC<16, ushort> CRC_16;
typedef PICRC<8, uchar>   CRC_8;

inline CRC_32 standardCRC_32() {return CRC_32(0x04C11DB7, true, 0xFFFFFFFF, 0xFFFFFFFF);}
inline CRC_16 standardCRC_16() {return CRC_16(0x8005, true, 0x0, 0x0);}
inline CRC_8 standardCRC_8() {return CRC_8(0xD5, true, 0x0, 0x0);}

#endif // CRC_H
