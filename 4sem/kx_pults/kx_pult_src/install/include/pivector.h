/*! \file pivector.h
 * \brief Dynamic array of any type
 *
 * This file declares PIVector
*/
/*
	PIP - Platform Independent Primitives
	Dynamic array of any type
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

#ifndef PIVECTOR_H
#define PIVECTOR_H

#include "piincludes.h"


#if !defined(PIP_CONTAINERS_STL) || defined(DOXYGEN)


template <typename T>
class PIVector {
public:
	PIVector(): piv_data(0), piv_size(0), piv_rsize(0) {
		//printf("new vector 1 %p (%s) ... !{\n", this, typeid(T).name());
		//printf("(s=%d, d=%p) }!\n", int(piv_size), piv_data);
	}
	PIVector(const T * data, size_t size): piv_data(0), piv_size(0), piv_rsize(0) {
		//printf("new vector 2 %p (%s) ... !{\n", this, typeid(T).name());
		alloc(size);
		newT(piv_data, data, piv_size);
		//printf("(s=%d, d=%p) }!\n", int(pid_size), pid_data);
	}
	PIVector(const PIVector<T> & other): piv_data(0), piv_size(0), piv_rsize(0) {
		//printf("new vector 2 %p (%s) ... !{\n", this, typeid(T).name());
		alloc(other.piv_size);
		newT(piv_data, other.piv_data, piv_size);
		//printf("(s=%d, d=%p) }!\n", int(piv_size), piv_data);
	}
	PIVector(size_t piv_size, const T & f = T()): piv_data(0), piv_size(0), piv_rsize(0) {
		//printf("new vector 3 %p (%s) ... !{\n", this, typeid(T).name());
		resize(piv_size, f);
		//printf("(s=%d, d=%p) }!\n", int(piv_size), piv_data);
	}
	~PIVector() {
		//printf("delete vector %p (%s) (s=%d, d=%p) ... ~{\n", this, typeid(T).name(), int(piv_size), piv_data);
		deleteT(piv_data, piv_size);
		dealloc();
		//deleteRaw(piv_tdata);
		_reset();
		//printf("}~\n");
	}

	PIVector<T> & operator =(const PIVector<T> & other) {
		if (this == &other) return *this;
		bool tj, oj;
		tj = (piv_size != 0 && piv_data == 0);// || (piv_size == 0 && piv_data != 0);
		oj = (other.piv_size != 0 && other.piv_data == 0);// || (other.piv_size == 0 && other.piv_data != 0);
		//printf("operator= (%p = %p) (s=%d, d=%p,  o.s=%d, o.d=%p) (%d, %d) ... o[\n", this, &other, int(piv_size), piv_data, int(other.piv_size), other.piv_data, int(tj), int(oj));
		if (tj) {
			printf("JUNK this\n");
			_reset();
		} else {
			clear();
		}
		/*if (piv_size == other.piv_size) {
			for (size_t i = 0; i < piv_size; ++i)
				piv_data[i] = other.piv_data[i];
			return *this;
		}*/
		if (!oj) {
			deleteT(piv_data, piv_size);
			alloc(other.piv_size);
			//zeroRaw(piv_data, piv_size);
			for (size_t i = 0; i < piv_size; ++i)
				elementNew(piv_data + i, other.piv_data[i]); //piv_data[i] = other.piv_data[i];
		} else {
			printf("JUNK other\n");
		}
		//printf("o]\n");
		return *this;
	}

	typedef T value_type;

	class iterator {
		friend class PIVector<T>;
	private:
		iterator(PIVector<T> * v, size_t p): parent(v), pos(p) {}
		PIVector<T> * parent;
		size_t pos;
	public:
		iterator(): parent(0), pos(0) {}
		T & operator *() {return (*parent)[pos];}
		const T & operator *() const {return (*parent)[pos];}
		void operator ++() {++pos;}
		void operator ++(int) {++pos;}
		void operator --() {--pos;}
		void operator --(int) {--pos;}
		bool operator ==(const iterator & it) const {return (pos == it.pos);}
		bool operator !=(const iterator & it) const {return (pos != it.pos);}
	};

	class const_iterator {
		friend class PIVector<T>;
	private:
		const_iterator(const PIVector<T> * v, size_t p): parent(v), pos(p) {}
		const PIVector<T> * parent;
		size_t pos;
	public:
		const_iterator(): parent(0), pos(0) {}
		//T & operator *() {return (*parent)[pos];}
		const T & operator *() const {return (*parent)[pos];}
		void operator ++() {++pos;}
		void operator ++(int) {++pos;}
		void operator --() {--pos;}
		void operator --(int) {--pos;}
		bool operator ==(const const_iterator & it) const {return (pos == it.pos);}
		bool operator !=(const const_iterator & it) const {return (pos != it.pos);}
	};

	class reverse_iterator {
		friend class PIVector<T>;
	private:
		reverse_iterator(PIVector<T> * v, size_t p): parent(v), pos(p) {}
		PIVector<T> * parent;
		size_t pos;
	public:
		reverse_iterator(): parent(0), pos(0) {}
		T & operator *() {return (*parent)[pos];}
		const T & operator *() const {return (*parent)[pos];}
		void operator ++() {--pos;}
		void operator ++(int) {--pos;}
		void operator --() {++pos;}
		void operator --(int) {++pos;}
		bool operator ==(const reverse_iterator & it) const {return (pos == it.pos);}
		bool operator !=(const reverse_iterator & it) const {return (pos != it.pos);}
	};

	class const_reverse_iterator {
		friend class PIVector<T>;
	private:
		const_reverse_iterator(const PIVector<T> * v, size_t p): parent(v), pos(p) {}
		const PIVector<T> * parent;
		size_t pos;
	public:
		const_reverse_iterator(): parent(0), pos(0) {}
		//T & operator *() {return (*parent)[pos];}
		const T & operator *() const {return (*parent)[pos];}
		void operator ++() {--pos;}
		void operator ++(int) {--pos;}
		void operator --() {++pos;}
		void operator --(int) {++pos;}
		bool operator ==(const const_reverse_iterator & it) const {return (pos == it.pos);}
		bool operator !=(const const_reverse_iterator & it) const {return (pos != it.pos);}
	};

	iterator begin() {return iterator(this, 0);}
	iterator end() {return iterator(this, piv_size);}
	const_iterator begin() const {return const_iterator(this, 0);}
	const_iterator end() const {return const_iterator(this, piv_size);}
	reverse_iterator rbegin() {return reverse_iterator(this, piv_size - 1);}
	reverse_iterator rend() {return reverse_iterator(this, -1);}
	const_reverse_iterator rbegin() const {return const_reverse_iterator(this, piv_size - 1);}
	const_reverse_iterator rend() const {return const_reverse_iterator(this, -1);}

	size_t size() const {return piv_size;}
	ssize_t size_s() const {return piv_size;}
	size_t length() const {return piv_size;}
	size_t capacity() const {return piv_rsize;}
	bool isEmpty() const {return (piv_size == 0);}

	T & operator [](size_t index) {return piv_data[index];}
	T & at(size_t index) {return piv_data[index];}
	const T & operator [](size_t index) const {return piv_data[index];}
	const T & at(size_t index) const {return piv_data[index];}
	T & back() {return piv_data[piv_size - 1];}
	const T & back() const {return piv_data[piv_size - 1];}
	T & front() {return piv_data[0];}
	const T & front() const {return piv_data[0];}
	bool operator ==(const PIVector<T> & t) const {if (piv_size != t.piv_size) return false; for (size_t i = 0; i < piv_size; ++i) if (t[i] != piv_data[i]) return false; return true;}
	bool operator !=(const PIVector<T> & t) const {if (piv_size != t.piv_size) return true; for (size_t i = 0; i < piv_size; ++i) if (t[i] != piv_data[i]) return true; return false;}
	bool contains(const T & v) const {for (size_t i = 0; i < piv_size; ++i) if (v == piv_data[i]) return true; return false;}
	int etries(const T & v) const {int ec = 0; for (size_t i = 0; i < piv_size; ++i) if (v == piv_data[i]) ++ec; return ec;}
	ssize_t indexOf(const T & v) const {for (ssize_t i = 0; i < piv_size; ++i) if (v == piv_data[i]) return i; return -1;}
	ssize_t lastIndexOf(const T & v) const {for (ssize_t i = piv_size - 1; i >= 0; --i) if (v == piv_data[i]) return i; return -1;}

	T * data(size_t index = 0) {return &(piv_data[index]);}
	const T * data(size_t index = 0) const {return &(piv_data[index]);}
	PIVector<T> & clear() {resize(0); return *this;}
	PIVector<T> & fill(const T & f = T()) {
		//if (sizeof(T) == 1) memset(piv_data, f, piv_size);
		deleteT(piv_data, piv_size);
		//zeroRaw(piv_data, piv_size);
		for (size_t i = 0; i < piv_size; ++i)
			elementNew(piv_data + i, f);
		return *this;
	}
	PIVector<T> & assign(const T & f = T()) {return fill(f);}
	PIVector<T> & assign(size_t new_size, const T & f) {resize(new_size); return fill(f);}
	PIVector<T> & resize(size_t new_size, const T & f = T()) {
		if (new_size < piv_size) {
			T * de = &(piv_data[new_size]);
			deleteT(de, piv_size - new_size);
			piv_size = new_size;
		}
		if (new_size > piv_size) {
			size_t os = piv_size;
			alloc(new_size);
			//if (sizeof(T) == 1) memset(&(piv_data[os]), f, ds);
			//zeroRaw(&(piv_data[os]), new_size - os);
			for (size_t i = os; i < new_size; ++i) elementNew(piv_data + i, f);
		}
		return *this;
	}
	PIVector<T> & reserve(size_t new_size) {if (new_size <= piv_rsize) return *this; size_t os = piv_size; alloc(new_size); piv_size = os; return *this;}

	PIVector<T> & insert(size_t index, const T & v = T()) {
		alloc(piv_size + 1);
		if (index < piv_size - 1) {
			size_t os = piv_size - index - 1;
			memmove(&(piv_data[index + 1]), &(piv_data[index]), os * sizeof(T));
		}
		//zeroRaw(&(piv_data[index]), 1);
		elementNew(piv_data + index, v);
		return *this;
	}
	PIVector<T> & insert(size_t index, const PIVector<T> & other) {
		if (other.isEmpty()) return *this;
		ssize_t os = piv_size - index;
		alloc(piv_size + other.piv_size);
		if (os > 0)
			memmove(&(piv_data[index + other.piv_size]), &(piv_data[index]), os * sizeof(T));
		newT(piv_data + index, other.piv_data, other.piv_size);
		return *this;
	}

	PIVector<T> & remove(size_t index, size_t count = 1) {
		if (count == 0) return *this;
		if (index + count >= piv_size) {
			resize(index);
			return *this;
		}
		size_t os = piv_size - index - count;
		deleteT(&(piv_data[index]), count);
		memmove(&(piv_data[index]), &(piv_data[index + count]), os * sizeof(T));
		piv_size -= count;
		return *this;
	}

	void swap(PIVector<T> & other) {
		piSwap<T*>(piv_data, other.piv_data);
		piSwap<size_t>(piv_size, other.piv_size);
		piSwap<size_t>(piv_rsize, other.piv_rsize);
	}

	typedef int (*CompareFunc)(const T * , const T * );
	static int compare_func(const T * t0, const T * t1) {return (*t0) < (*t1) ? -1 : ((*t0) == (*t1) ? 0 : 1);}
	PIVector<T> & sort(CompareFunc compare = compare_func) {qsort(piv_data, piv_size, sizeof(T), (int(*)(const void * , const void * ))compare); return *this;}

	PIVector<T> & enlarge(llong piv_size) {llong ns = size_s() + piv_size; if (ns <= 0) clear(); else resize(size_t(ns)); return *this;}

	PIVector<T> & removeOne(const T & v) {for (size_t i = 0; i < piv_size; ++i) if (piv_data[i] == v) {remove(i); return *this;} return *this;}
	PIVector<T> & removeAll(const T & v) {for (llong i = 0; i < piv_size; ++i) if (piv_data[i] == v) {remove(i); --i;} return *this;}

	PIVector<T> & push_back(const T & v) {alloc(piv_size + 1); elementNew(piv_data + piv_size - 1, v); return *this;}
	PIVector<T> & append(const T & v) {return push_back(v);}
	PIVector<T> & operator <<(const T & v) {return push_back(v);}
	PIVector<T> & operator <<(const PIVector<T> & other) {
		size_t ps = piv_size;
		alloc(piv_size + other.piv_size);
		newT(piv_data + ps, other.piv_data, other.piv_size);
		return *this;
	}

	PIVector<T> & push_front(const T & v) {insert(0, v); return *this;}
	PIVector<T> & prepend(const T & v) {return push_front(v);}

	PIVector<T> & pop_back() {if (piv_size == 0) return *this; resize(piv_size - 1); return *this;}
	PIVector<T> & pop_front() {if (piv_size == 0) return *this; remove(0); return *this;}

	T take_back() {T t(back()); pop_back(); return t;}
	T take_front() {T t(front()); pop_front(); return t;}

	template <typename ST>
	PIVector<ST> toType() const {PIVector<ST> ret(piv_size); for (uint i = 0; i < piv_size; ++i) ret[i] = ST(piv_data[i]); return ret;}

private:
	void _reset() {piv_size = piv_rsize = 0; piv_data = 0;}
	size_t asize(size_t s) {
		if (s == 0) return 0;
		if (piv_rsize + piv_rsize >= s && piv_rsize < s)
			return piv_rsize + piv_rsize;
		size_t t = 0, s_ = s - 1;
		while (s_ >> t) ++t;
		return (1 << t);
	}
	inline void newT(T * dst, const T * src, size_t s) {
		for (size_t i = 0; i < s; ++i)
			elementNew(dst + i, src[i]);
	}
	T * newRaw(size_t s) {
		//cout << std::dec << "  ![("<<this<<")newRaw " << s << " elements ... <\n" << endl;
		//uchar * ret = new uchar[s * sizeof(T)];
		uchar * ret = (uchar*)(malloc(s * sizeof(T)));//new uchar[];
		//zeroRaw((T*)ret, s);
		//cout << std::hex << "  > (new 0x" << (llong)ret << ") ok]!" << endl;
		return (T*)ret;
	}
	/*void reallocRawTemp(size_t s) {
		if (piv_tdata == 0) piv_tdata = (T*)(malloc(s * sizeof(T)));
		else piv_tdata = (T*)(realloc(piv_tdata, s * sizeof(T)));
	}*/
	inline void deleteT(T * d, size_t sz) {
		//cout << "  ~[("<<this<<")deleteT " << std::dec << sz << " elements " << std::hex << "0x" << (llong)d << " ... <\n" << endl;
		if ((uchar*)d != 0) {
			for (size_t i = 0; i < sz; ++i)
				elementDelete(d[i]);
			//zeroRaw(d, sz);
		}
		//cout << "  > ok]~" << endl;
	}
	inline void deleteRaw(T *& d) {
		//cout << "  ~[("<<this<<")deleteRaw " << std::dec << piv_rsize << " elements " << std::hex << "0x" << (llong)d << " ... <\n" << endl;
		if ((uchar*)d != 0) free((uchar*)d);
		d = 0;
		//cout << "  > ok]~" << endl;
	}
	inline void zeroRaw(T * d, size_t s) {
		//cout << "  ~[("<<this<<")zeroRaw " << std::dec << s << " elements " << std::hex << "0x" << (llong)d << " ... <\n" << endl;
		if ((uchar*)d != 0) memset(d, 0, s*sizeof(T));
		//cout << "  > ok]~" << endl;
	}
	inline void elementNew(T * to, const T & from) {new(to)T(from);}
	inline void elementDelete(T & from) {from.~T();}
	void dealloc() {deleteRaw(piv_data);}
	inline void alloc(size_t new_size) {
		if (new_size <= piv_rsize) {
			piv_size = new_size;
			return;
		}
		//int os = piv_size;
		piv_size = new_size;
		size_t as = asize(new_size);
		if (as == piv_rsize) return;

		//cout << std::hex << "  ![("<<this<<")realloc " << piv_data << " data ... <\n" << endl;
		T * p_d = (T*)(realloc(piv_data, as*sizeof(T)));
		assert(p_d);
		piv_data = p_d;
		//zeroRaw(&(piv_data[os]), as - os);
		piv_rsize = as;
		//cout << std::hex << "  > (new 0x" << (llong)piv_data << ") ok]!" << endl;
		/*piv_rsize = as;
		T * pd = newRaw(piv_rsize);
		if (os > 0 && piv_data != 0) {
			memcpy(pd, piv_data, os * sizeof(T));
			deleteRaw(piv_data);
		}
		piv_data = pd;*/
	}

	T * piv_data;
	volatile size_t piv_size, piv_rsize;
};
/*
#define __PIVECTOR_SIMPLE_FUNCTIONS__(T) \
	template<> inline PIVector<T>::~PIVector() {dealloc(); _reset();} \
	template<> inline PIVector<T> & PIVector<T>::push_back(const T & v) {alloc(piv_size + 1); piv_data[piv_size - 1] = v; return *this;} \
	template<> inline PIVector<T> & PIVector<T>::fill(const T & f) { \
		for (size_t i = 0; i < piv_size; ++i) \
			piv_data[i] = f; \
		return *this; \
	} \
	template<> inline PIVector<T> & PIVector<T>::resize(size_t new_size, const T & f) { \
		if (new_size < piv_size) \
			piv_size = new_size; \
		if (new_size > piv_size) { \
			size_t os = piv_size; \
			alloc(new_size); \
			for (size_t i = os; i < new_size; ++i) piv_data[i] = f; \
		} \
		return *this; \
	} \
	template<> inline PIVector<T> & PIVector<T>::insert(size_t index, const T & v) { \
		alloc(piv_size + 1); \
		if (index < piv_size - 1) { \
			size_t os = piv_size - index - 1; \
			memmove(&(piv_data[index + 1]), &(piv_data[index]), os * sizeof(T)); \
		} \
		piv_data[index] = v; \
		return *this; \
	} \
	template<> inline PIVector<T> & PIVector<T>::remove(size_t index, size_t count) { \
		if (count == 0) return *this; \
		if (index + count >= piv_size) { \
			resize(index); \
			return *this; \
		} \
		size_t os = piv_size - index - count; \
		memmove(&(piv_data[index]), &(piv_data[index + count]), os * sizeof(T)); \
		piv_size -= count; \
		return *this; \
	}

__PIVECTOR_SIMPLE_FUNCTIONS__(char)
__PIVECTOR_SIMPLE_FUNCTIONS__(uchar)
__PIVECTOR_SIMPLE_FUNCTIONS__(short)
__PIVECTOR_SIMPLE_FUNCTIONS__(ushort)
__PIVECTOR_SIMPLE_FUNCTIONS__(int)
__PIVECTOR_SIMPLE_FUNCTIONS__(uint)
__PIVECTOR_SIMPLE_FUNCTIONS__(long)
__PIVECTOR_SIMPLE_FUNCTIONS__(ulong)
__PIVECTOR_SIMPLE_FUNCTIONS__(llong)
__PIVECTOR_SIMPLE_FUNCTIONS__(ullong)
__PIVECTOR_SIMPLE_FUNCTIONS__(float)
__PIVECTOR_SIMPLE_FUNCTIONS__(double)
__PIVECTOR_SIMPLE_FUNCTIONS__(ldouble)*/
#define __PIVECTOR_SIMPLE_TYPE__(T) \
	template<> inline void PIVector<T>::newT(T * dst, const T * src, size_t s) {memcpy(dst, src, s * sizeof(T));} \
	template<> inline void PIVector<T>::deleteT(T * d, size_t sz) {;} \
	template<> inline void PIVector<T>::elementNew(T * to, const T & from) {(*to) = from;} \
	template<> inline void PIVector<T>::elementDelete(T & from) {;}

#else


template<typename T, typename Allocator = std::allocator<T> >
class PIP_EXPORT PIVector: public vector<T, Allocator> {
	typedef PIVector<T, Allocator> _CVector;
	typedef vector<T, Allocator> _stlc;
public:

	PIVector() {piMonitor.containers++;}
	PIVector(uint size, const T & value = T()) {piMonitor.containers++; _stlc::resize(size, value);}
	~PIVector() {piMonitor.containers--;}

	const T & at(uint index) const {return (*this)[index];}
	T & at(uint index) {return (*this)[index];}
	const T * data(uint index = 0) const {return &(*this)[index];}
	T * data(uint index = 0) {return &(*this)[index];}

#ifdef DOXYGEN
	uint size() const;
#endif

	int size_s() const {return static_cast<int>(_stlc::size());}
	bool isEmpty() const {return _stlc::empty();}
	bool has(const T & t) const {for (typename _stlc::const_iterator i = _stlc::begin(); i != _stlc::end(); ++i) if (t == *i) return true; return false;}
	int etries(const T & t) const {int ec = 0; for (typename _stlc::const_iterator i = _stlc::begin(); i != _stlc::end(); ++i) if (t == *i) ++ec; return ec;}

	typedef int (*CompareFunc)(const T * , const T * );

	static int compare_func(const T * t0, const T * t1) {return (*t0) == (*t1) ? 0 : ((*t0) < (*t1) ? -1 : 1);}
#ifdef DOXYGEN

	void resize(uint size, const T & new_type = T());
	PIVector<T, Allocator> & enlarge(uint size);
	void clear();
	PIVector<T, Allocator> & sort(CompareFunc compare = compare_func) {qsort(&at(0), _stlc::size(), sizeof(T), (int(*)(const void * , const void * ))compare); return *this;}
	PIVector<T, Allocator> & fill(const T & t) {_stlc::assign(_stlc::size(), t); return *this;}
	T & back();
	const T & back() const;
	T & front();
	const T & front() const;
	PIVector<T, Allocator> & push_back(const T & t);
	PIVector<T, Allocator> & push_front(const T & t) {_stlc::insert(_stlc::begin(), t); return *this;}
	PIVector<T, Allocator> & pop_back();
	PIVector<T, Allocator> & pop_front() {_stlc::erase(_stlc::begin()); return *this;}
	T take_back() {T t(_stlc::back()); _stlc::pop_back(); return t;}
	T take_front() {T t(_stlc::front()); pop_front(); return t;}
	PIVector<T, Allocator> & remove(uint index) {_stlc::erase(_stlc::begin() + index); return *this;}
	PIVector<T, Allocator> & remove(uint index, uint count) {_stlc::erase(_stlc::begin() + index, _stlc::begin() + index + count); return *this;}
	PIVector<T, Allocator> & removeOne(const T & v) {for (typename _stlc::iterator i = _stlc::begin(); i != _stlc::end(); ++i) if (v == *i) {_stlc::erase(i); return *this;} return *this;}
	PIVector<T, Allocator> & removeAll(const T & v) {for (typename _stlc::iterator i = _stlc::begin(); i != _stlc::end(); ++i) if (v == *i) {_stlc::erase(i); --i;} return *this;}
	PIVector<T, Allocator> & insert(uint pos, const T & t) {_stlc::insert(_stlc::begin() + pos, t); return *this;}
	PIVector<T, Allocator> & insert(uint pos, const PIVector<T, Allocator> & t) {_stlc::insert(_stlc::begin() + pos, t.begin(), t.end()); return *this;}
	T & operator [](uint index);
	const T & operator [](uint index) const;
	PIVector<T, Allocator> & operator <<(const T & t) {_stlc::push_back(t); return *this;}
	PIVector<T, Allocator> & operator <<(const PIVector<T, Allocator> & t) {for (typename _stlc::const_iterator i = t.begin(); i != t.end(); i++) _stlc::push_back(*i); return *this;}
	bool operator ==(const PIVector<T, Allocator> & t) {for (uint i = 0; i < _stlc::size(); ++i) if (t[i] != at(i)) return false; return true;}
	bool operator !=(const PIVector<T, Allocator> & t) {for (uint i = 0; i < _stlc::size(); ++i) if (t[i] != at(i)) return true; return false;}
	bool contains(const T & v) const {for (uint i = 0; i < _stlc::size(); ++i) if (v == at(i)) return true; return false;}

#else
	_CVector & enlarge(int size_) {int ns = size_s() + size_; if (ns <= 0) _stlc::clear(); else _stlc::resize(ns); return *this;}
	_CVector & sort(CompareFunc compare = compare_func) {qsort(&at(0), _stlc::size(), sizeof(T), (int(*)(const void * , const void * ))compare); return *this;}
	_CVector & fill(const T & t) {_stlc::assign(_stlc::size(), t); return *this;}
	_CVector & pop_front() {_stlc::erase(_stlc::begin()); return *this;}
	_CVector & push_front(const T & t) {_stlc::insert(_stlc::begin(), t); return *this;}
	T take_front() {T t(_stlc::front()); pop_front(); return t;}
	T take_back() {T t(_stlc::back()); _stlc::pop_back(); return t;}
	_CVector & remove(uint index) {_stlc::erase(_stlc::begin() + index); return *this;}
	_CVector & remove(uint index, uint count) {_stlc::erase(_stlc::begin() + index, _stlc::begin() + index + count); return *this;}
	_CVector & removeOne(const T & v) {for (typename _stlc::iterator i = _stlc::begin(); i != _stlc::end(); ++i) if (v == *i) {_stlc::erase(i); return *this;} return *this;}
	_CVector & removeAll(const T & v) {for (typename _stlc::iterator i = _stlc::begin(); i != _stlc::end(); ++i) if (v == *i) {_stlc::erase(i); --i;} return *this;}
	_CVector & insert(uint pos, const T & t) {_stlc::insert(_stlc::begin() + pos, t); return *this;}
	_CVector & insert(uint pos, const _CVector & t) {_stlc::insert(_stlc::begin() + pos, t.begin(), t.end()); return *this;}
	_CVector & operator <<(const T & t) {_stlc::push_back(t); return *this;}
	_CVector & operator <<(const _CVector & t) {for (typename _stlc::const_iterator i = t.begin(); i != t.end(); i++) _stlc::push_back(*i); return *this;}
	bool operator ==(const _CVector & t) {for (uint i = 0; i < _stlc::size(); ++i) if (t[i] != at(i)) return false; return true;}
	bool operator !=(const _CVector & t) {for (uint i = 0; i < _stlc::size(); ++i) if (t[i] != at(i)) return true; return false;}
	bool contains(const T & v) const {for (uint i = 0; i < _stlc::size(); ++i) if (v == at(i)) return true; return false;}
#endif
};

#define __PIVECTOR_SIMPLE_TYPE__(T)


#endif


template<typename T>
inline std::ostream & operator <<(std::ostream & s, const PIVector<T> & v) {s << "{"; for (size_t i = 0; i < v.size(); ++i) {s << v[i]; if (i < v.size() - 1) s << ", ";} s << "}"; return s;}

template<typename T>
inline PICout operator <<(PICout s, const PIVector<T> & v) {s.space(); s.setControl(0, true); s << "{"; for (size_t i = 0; i < v.size(); ++i) {s << v[i]; if (i < v.size() - 1) s << ", ";} s << "}"; s.restoreControl(); return s;}


#endif // PIVECTOR_H
