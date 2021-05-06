/*! \file pideque.h
 * \brief Dynamic array of any type
 *
 * This file declares PIDeque
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

#ifndef PIDEQUE_H
#define PIDEQUE_H

#include "piincludes.h"
#include "piintrospection_proxy.h"


#if !defined(PIP_CONTAINERS_STL) || defined(DOXYGEN)


template <typename T>
class PIDeque {
public:
	PIDeque(): pid_data(0), pid_size(0), pid_rsize(0), pid_start(0) {
		PIINTROSPECTION_CONTAINER_NEW()
		//printf("new vector 1 %p (%s) ... !{\n", this, typeid(T).name());
		//printf("(s=%d, d=%p) }!\n", int(pid_size), pid_data);
	}
	PIDeque(const PIDeque<T> & other): pid_data(0), pid_size(0), pid_rsize(0), pid_start(0) {
		PIINTROSPECTION_CONTAINER_NEW()
		//printf("new vector 2 %p (%s) ... !{\n", this, typeid(T).name());
		alloc(other.pid_size, true);
		newT(pid_data + pid_start, other.pid_data + other.pid_start, pid_size);
		//printf("(s=%d, d=%p) }!\n", int(pid_size), pid_data);
	}
	PIDeque(const T * data, size_t size): pid_data(0), pid_size(0), pid_rsize(0), pid_start(0) {
		PIINTROSPECTION_CONTAINER_NEW()
		//printf("new vector 2 %p (%s) ... !{\n", this, typeid(T).name());
		alloc(size, true);
		newT(pid_data + pid_start, data, pid_size);
		//printf("(s=%d, d=%p) }!\n", int(pid_size), pid_data);
	}
	PIDeque(size_t pid_size, const T & f = T()): pid_data(0), pid_size(0), pid_rsize(0), pid_start(0) {
		PIINTROSPECTION_CONTAINER_NEW()
		//printf("new vector 3 %p (%s) ... !{\n", this, typeid(T).name());
		resize(pid_size, f);
		//printf("(s=%d, d=%p) }!\n", int(pid_size), pid_data);
	}
	~PIDeque() {
		PIINTROSPECTION_CONTAINER_DELETE()
		PIINTROSPECTION_CONTAINER_FREE((pid_rsize)*sizeof(T))
		//printf("delete deque %p (%s) (s=%d, rs=%d, st=%d, d=%p) ... ~{\n", this, typeid(T).name(), int(pid_size), int(pid_rsize), int(pid_start), pid_data);
		deleteT(pid_data + pid_start, pid_size);
		dealloc();
		//deleteRaw(pid_tdata);
		_reset();
		//printf("}~\n");
	}

	PIDeque<T> & operator =(const PIDeque<T> & other) {
		if (this == &other) return *this;
		deleteT(pid_data + pid_start, pid_size);
		alloc(other.pid_size, true);
		newT(pid_data + pid_start, other.pid_data + other.pid_start, pid_size);
		return *this;
	}

	typedef T value_type;

	class iterator {
		friend class PIDeque<T>;
	private:
		iterator(PIDeque<T> * v, size_t p): parent(v), pos(p) {}
		PIDeque<T> * parent;
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
		friend class PIDeque<T>;
	private:
		const_iterator(const PIDeque<T> * v, size_t p): parent(v), pos(p) {}
		const PIDeque<T> * parent;
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
		friend class PIDeque<T>;
	private:
		reverse_iterator(PIDeque<T> * v, size_t p): parent(v), pos(p) {}
		PIDeque<T> * parent;
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
		friend class PIDeque<T>;
	private:
		const_reverse_iterator(const PIDeque<T> * v, size_t p): parent(v), pos(p) {}
		const PIDeque<T> * parent;
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
	iterator end() {return iterator(this, pid_size);}
	const_iterator begin() const {return const_iterator(this, 0);}
	const_iterator end() const {return const_iterator(this, pid_size);}
	reverse_iterator rbegin() {return reverse_iterator(this, pid_size - 1);}
	reverse_iterator rend() {return reverse_iterator(this, -1);}
	const_reverse_iterator rbegin() const {return const_reverse_iterator(this, pid_size - 1);}
	const_reverse_iterator rend() const {return const_reverse_iterator(this, -1);}

	size_t size() const {return pid_size;}
	ssize_t size_s() const {return pid_size;}
	size_t length() const {return pid_size;}
	size_t capacity() const {return pid_rsize;}
	bool isEmpty() const {return (pid_size == 0);}

	T & operator [](size_t index) {return pid_data[pid_start + index];}
	T & at(size_t index) {return pid_data[pid_start + index];}
	const T & operator [](size_t index) const {return pid_data[pid_start + index];}
	const T & at(size_t index) const {return pid_data[pid_start + index];}
	T & back() {return pid_data[pid_start + pid_size - 1];}
	const T & back() const {return pid_data[pid_start + pid_size - 1];}
	T & front() {return pid_data[pid_start];}
	const T & front() const {return pid_data[pid_start];}
	bool operator ==(const PIDeque<T> & t) const {if (pid_size != t.pid_size) return false; for (size_t i = 0; i < pid_size; ++i) if (t[i] != (*this)[i]) return false; return true;}
	bool operator !=(const PIDeque<T> & t) const {if (pid_size != t.pid_size) return true; for (size_t i = 0; i < pid_size; ++i) if (t[i] != (*this)[i]) return true; return false;}
	bool contains(const T & v) const {for (size_t i = pid_start; i < pid_start + pid_size; ++i) if (v == pid_data[i]) return true; return false;}
	int etries(const T & v) const {int ec = 0; for (size_t i = pid_start; i < pid_start + pid_size; ++i) if (v == pid_data[i]) ++ec; return ec;}

	T * data(size_t index = 0) {return &(pid_data[pid_start + index]);}
	const T * data(size_t index = 0) const {return &(pid_data[pid_start + index]);}
	PIDeque<T> & clear() {resize(0); return *this;}
	PIDeque<T> & fill(const T & f = T()) {
		//if (sizeof(T) == 1) memset(pid_data, f, pid_size);
		deleteT(pid_data + pid_start, pid_size);
		//zeroRaw(pid_data, pid_size);
		for (size_t i = pid_start; i < pid_start + pid_size; ++i)
			elementNew(pid_data + i, f);
		return *this;
	}
	PIDeque<T> & assign(const T & f = T()) {return fill(f);}
	PIDeque<T> & assign(size_t new_size, const T & f) {resize(new_size); return fill(f);}
	PIDeque<T> & resize(size_t new_size, const T & f = T()) {
		if (new_size < pid_size) {
			deleteT(&(pid_data[new_size + pid_start]), pid_size - new_size);
			pid_size = new_size;
		}
		if (new_size > pid_size) {
			size_t os = pid_size;
			alloc(new_size, true);
			//if (sizeof(T) == 1) memset(&(pid_data[os]), f, ds);
			//zeroRaw(&(pid_data[os]), new_size - os);
			PIINTROSPECTION_CONTAINER_USED((new_size-os)*sizeof(T))
			for (size_t i = os + pid_start; i < new_size + pid_start; ++i) elementNew(pid_data + i, f);
		}
		return *this;
	}
	PIDeque<T> & reserve(size_t new_size) {
		if (new_size <= pid_rsize) return *this;
		size_t os = pid_size;
		alloc(new_size, true);
		pid_size = os;
		return *this;
	}

	PIDeque<T> & insert(size_t index, const T & v = T()) {
		bool dir = pid_rsize <= 2 ? true : (index >= pid_rsize / 2 ? true : false);
		//piCout << "insert" << dir << index << pid_size << pid_rsize << pid_start << "!<";
		if (dir) {
			alloc(pid_size + 1, true);
			if (index < pid_size - 1) {
				size_t os = pid_size - index - 1;
				memmove(&(pid_data[index + pid_start + 1]), &(pid_data[index + pid_start]), os * sizeof(T));
			}
		} else {
			alloc(pid_size + 1, false, -1);
			//piCout << "insert front" << pid_size << pid_rsize << pid_start << "!<";
			if (index > 0)
				memmove(&(pid_data[pid_start]), &(pid_data[pid_start + 1]), index * sizeof(T));
		}
		//piCout << "insert" << pid_start << index << (pid_start + ssize_t(index)) << pid_size << ">!";
		PIINTROSPECTION_CONTAINER_USED(sizeof(T))
		elementNew(pid_data + pid_start + index, v);
		return *this;
	}
	PIDeque<T> & insert(size_t index, const PIDeque<T> & other) {
		if (other.isEmpty()) return *this;
		bool dir = pid_rsize <= 2 ? true : (index >= pid_rsize / 2 ? true : false);
		//piCout << this << "insert" << dir << index << pid_size << pid_rsize << pid_start << " <- " << other.size() << "!<";
		if (dir) {
			ssize_t os = pid_size - index;
			alloc(pid_size + other.pid_size, true);
			if (os > 0)
				memmove(&(pid_data[index + pid_start + other.pid_size]), &(pid_data[index + pid_start]), os * sizeof(T));
		} else {
			//if (pid_start < other.pid_size) pid_start = 0;
			//piCout << this << "  insert offseted start ba" << pid_start << pid_size << pid_rsize;
			alloc(pid_size + other.pid_size, false, -other.pid_size);
			//piCout << this << "  insert offseted start aa" << pid_start << pid_size << pid_rsize;
			//piCout << this << "  insert front" << pid_size << pid_rsize << pid_start << "!<";
			if (index > 0)
				memmove(&(pid_data[pid_start]), &(pid_data[pid_start + other.pid_size]), index * sizeof(T));
		}
		//piCout << this << "insert" << pid_start << index << (pid_start + ssize_t(index)) << pid_size << ">!";
		newT(pid_data + pid_start + index, other.pid_data + other.pid_start, other.pid_size);
		return *this;
	}

	PIDeque<T> & remove(size_t index, size_t count = 1) {
		if (count == 0) return *this;
		if (index + count >= pid_size) {
			resize(index);
			return *this;
		}
		size_t os = pid_size - index - count;
		deleteT(&(pid_data[index + pid_start]), count);
		if (os <= index) {
		//if (true) {
			if (os > 0) memmove(&(pid_data[index + pid_start]), &(pid_data[index + pid_start + count]), os * sizeof(T));
		} else {
			if (index > 0) memmove(&(pid_data[pid_start + count]), &(pid_data[pid_start]), index * sizeof(T));
			pid_start += count;
		}
		pid_size -= count;
		return *this;
	}

	void swap(PIDeque<T> & other) {
		piSwap<T*>(pid_data, other.pid_data);
		piSwap<size_t>(pid_size, other.pid_size);
		piSwap<size_t>(pid_rsize, other.pid_rsize);
		piSwap<size_t>(pid_start, other.pid_start);
	}

	typedef int (*CompareFunc)(const T * , const T * );
	static int compare_func(const T * t0, const T * t1) {return (*t0) < (*t1) ? -1 : ((*t0) == (*t1) ? 0 : 1);}
	PIDeque<T> & sort(CompareFunc compare = compare_func) {qsort(pid_data + pid_start, pid_size, sizeof(T), (int(*)(const void * , const void * ))compare); return *this;}

	PIDeque<T> & enlarge(llong pid_size) {llong ns = size_s() + pid_size; if (ns <= 0) clear(); else resize(size_t(ns)); return *this;}

	PIDeque<T> & removeOne(const T & v) {for (size_t i = 0; i < pid_size; ++i) if (pid_data[i + pid_start] == v) {remove(i); return *this;} return *this;}
	PIDeque<T> & removeAll(const T & v) {for (llong i = 0; i < pid_size; ++i) if (pid_data[i + pid_start] == v) {remove(i); --i;} return *this;}

	PIDeque<T> & push_back(const T & v) {alloc(pid_size + 1, true); PIINTROSPECTION_CONTAINER_USED(sizeof(T)); elementNew(pid_data + pid_start + pid_size - 1, v); return *this;}
	PIDeque<T> & append(const T & v) {return push_back(v);}
	PIDeque<T> & operator <<(const T & v) {return push_back(v);}
	PIDeque<T> & operator <<(const PIDeque<T> & t) {
		size_t ps = pid_size;
		alloc(pid_size + t.pid_size, true);
		newT(pid_data + ps + pid_start, t.pid_data + t.pid_start, t.pid_size);
		return *this;
	}

	PIDeque<T> & push_front(const T & v) {insert(0, v); return *this;}
	PIDeque<T> & prepend(const T & v) {return push_front(v);}

	PIDeque<T> & pop_back() {if (pid_size == 0) return *this; resize(pid_size - 1); return *this;}
	PIDeque<T> & pop_front() {if (pid_size == 0) return *this; remove(0); return *this;}

	T take_back() {T t(back()); pop_back(); return t;}
	T take_front() {T t(front()); pop_front(); return t;}

	template <typename ST>
	PIDeque<ST> toType() const {PIDeque<ST> ret(pid_size); for (uint i = 0; i < pid_size; ++i) ret[i] = ST(pid_data[i + pid_start]); return ret;}

private:
	void _reset() {pid_size = pid_rsize = pid_start = 0; pid_data = 0;}
	/*void * qmemmove(void * dst, void * src, size_t size) {
		if (piAbs<ssize_t>(ssize_t(dst) - ssize_t(src)) >= size)
			memcpy(dst, src, size);
		else {
			char * tb = new char[size];
			memcpy(tb, src, size);
			memcpy(dst, tb, size);
			delete tb;
		}
		return dst;
	}*/
	inline size_t asize(ssize_t s) {
		if (s <= 0) return 0;
		if (pid_rsize + pid_rsize >= size_t(s) && pid_rsize < size_t(s))
			return pid_rsize + pid_rsize;
		size_t t = 0, s_ = size_t(s) - 1;
		while (s_ >> t) ++t;
		return (1 << t);
	}
	inline void newT(T * dst, const T * src, size_t s) {
		PIINTROSPECTION_CONTAINER_USED(s*sizeof(T))
		for (size_t i = 0; i < s; ++i)
			elementNew(dst + i, src[i]);
	}
	static T * newRaw(size_t s) {
		//std::cout << std::dec << "  ![("<<this<<")newRaw " << s << " elements ... <" << std::endl;
		//uchar * ret = new uchar[s * sizeof(T)];
		uchar * ret = (uchar*)(malloc(s * sizeof(T)));//new uchar[];
		//zeroRaw((T*)ret, s);
		//std::cout << std::hex << "  > (new 0x" << (llong)ret << ") ok]!" << std::endl;
		return (T*)ret;
	}
	/*void reallocRawTemp(size_t s) {
		if (pid_tdata == 0) pid_tdata = (T*)(malloc(s * sizeof(T)));
		else pid_tdata = (T*)(realloc(pid_tdata, s * sizeof(T)));
	}*/
	inline void deleteT(T * d, size_t sz) {
		PIINTROSPECTION_CONTAINER_UNUSED(sz*sizeof(T))
		//std::cout << "  ~[("<<this<<")deleteT " << std::dec << sz << " elements " << " start " << pid_start << std::hex << " 0x" << (llong)d << " ... <" << std::endl;
		if ((uchar*)d != 0) {
			for (size_t i = 0; i < sz; ++i)
				elementDelete(d[i]);
			//zeroRaw(d, sz);
		}
		//cout << "  > ok]~" << endl;
	}
	static void deleteRaw(T *& d) {
		//cout << "  ~[("<<this<<")deleteRaw " << std::dec << pid_rsize << " elements " << std::hex << "0x" << (llong)d << " ... <\n" << endl;
		if ((uchar*)d != 0) free((uchar*)d);
		d = 0;
		//cout << "  > ok]~" << endl;
	}
	static void zeroRaw(T * d, size_t s) {
		//cout << "  ~[("<<this<<")zeroRaw " << std::dec << s << " elements " << std::hex << "0x" << (llong)d << " ... <\n" << endl;
		if ((uchar*)d != 0) memset(d, 0, s*sizeof(T));
		//cout << "  > ok]~" << endl;
	}
	inline void elementNew(T * to, const T & from) {new(to)T(from);}
	inline void elementDelete(T & from) {from.~T();}
	void dealloc() {deleteRaw(pid_data);}
	inline void checkMove(bool direction) {
		if (pid_size >= 4 && pid_size < pid_rsize / 6) {
			/*if (direction) {
				if (pid_start >= 4 && pid_start > pid_size + pid_size && pid_start > pid_rsize / 2) {
			piCout << (int)this << "checkMove" << direction << pid_start << (int)pid_data << pid_rsize << pid_size;
					piCout << (int)this << "move from" << pid_start << "to" << pid_size << "," << (int)pid_data << pid_rsize << pid_size;
					memmove(pid_data + pid_size, pid_data + pid_start, pid_size * sizeof(T));
					pid_start = pid_size;
				}
			} else {
				if (ssize_t(pid_start) < ssize_t(pid_rsize) - pid_size - pid_size && ssize_t(pid_start) < ssize_t(pid_rsize / 2) - pid_size) {
			piCout << (int)this << "checkMove" << direction << pid_start << (int)pid_data << pid_rsize << pid_size;
					piCout << (int)this << "move from" << pid_start << "to" << (ssize_t(pid_rsize) - pid_size) << "," << (int)pid_data << pid_rsize << pid_size;
					memmove(pid_data + ssize_t(pid_rsize) - pid_size - pid_size, pid_data + pid_start, pid_size * sizeof(T));
					pid_start = ssize_t(pid_rsize) - pid_size - pid_size;
				}
			}*/
			//printf("(%p) check move st=%d sz=%d rs=%d\n", this, pid_start, pid_size, pid_rsize);
			if (pid_start < pid_size + pid_size || pid_start > pid_rsize - pid_size - pid_size) {
				ssize_t ns = (pid_rsize - pid_size) / 2;
				if (pid_start != ns) {
					//printf("(%p) move %d -> %d\n", this, pid_start, ns);
					memmove(pid_data + ns, pid_data + pid_start, pid_size * sizeof(T));
					pid_start = ns;
				}
			}
		}
	}
	inline void alloc(size_t new_size, bool direction, ssize_t start_offset = 0) { // direction == true -> alloc forward
		if (direction) {
			if (pid_start + new_size <= pid_rsize) {
				pid_size = new_size;
				checkMove(direction);
				return;
			}
			pid_size = new_size;
			size_t as = asize(pid_start + new_size);
			if (as != pid_rsize) {
				//printf("(%p) realloc %d -> %d (%p)\n", this, pid_rsize, as, pid_data);
				PIINTROSPECTION_CONTAINER_ALLOC((as-pid_rsize)*sizeof(T))
				T * p_d = (T*)(realloc(pid_data, as*sizeof(T)));
				assert(p_d);
				pid_data = p_d;
				pid_rsize = as;
				//printf("(%p) realloc done (%p)\n", this, pid_data);
			}
		} else {
			size_t as;
			//piCout << "INS ba" << *this;
			if (pid_start + start_offset < 0)
				as = asize(pid_rsize - start_offset);
			else as = pid_rsize;
			//printf("%X alloc %d %d\n", this, pid_rsize, start_offset);
			//printf("%X alloc %d %d %d %d %d %d\n", this, new_size, pid_size, pid_rsize, as, pid_start, start_offset);
			if (as > pid_rsize) {
				//printf("%X alloc new size %d\n", this, as);
				//cout << std::hex << "  ![("<<this<<")realloc " << pid_data << " data ... <\n" << endl;
				T * td = newRaw(as);
				ssize_t ns = pid_start + as - pid_rsize;
				//printf("%X pid_start ost=%d ors=%d nst=%d nrs=%d\n", this, pid_start, pid_rsize, ns, as);
				PIINTROSPECTION_CONTAINER_ALLOC((as-pid_rsize)*sizeof(T))
				if (pid_rsize > 0 && pid_data != 0) {
					//printf("%X copy from %p + %d  to  %p + %d  %d el\n", this, pid_data, pid_start, td, ns, pid_size);
					memcpy(td + ns, pid_data + pid_start, pid_size * sizeof(T));
					deleteRaw(pid_data);
				}
				pid_data = td;
				pid_rsize = as;
				pid_start = ns;
			//piCout << "INS aa" << *this;
			}
			pid_start += start_offset;
			pid_size = new_size;
			checkMove(direction);
		}
		//checkMove(direction);
		//printf("%X alloc new start %d\n", this, pid_start);
	}

	T * pid_data;
	volatile size_t pid_size, pid_rsize;
	volatile ssize_t pid_start;
};

#define __PIDEQUE_SIMPLE_TYPE__(T) \
	template<> inline void PIDeque<T>::newT(T * dst, const T * src, size_t s) {PIINTROSPECTION_CONTAINER_USED(s*sizeof(T)); memcpy(dst, src, s * sizeof(T));} \
	template<> inline void PIDeque<T>::deleteT(T * d, size_t sz) {PIINTROSPECTION_CONTAINER_UNUSED(sz*sizeof(T));} \
	template<> inline void PIDeque<T>::elementNew(T * to, const T & from) {(*to) = from;} \
	template<> inline void PIDeque<T>::elementDelete(T & from) {;}

#else


template<typename Type, typename Allocator = std::allocator<Type> >
class PIP_EXPORT PIDeque: public deque<Type, Allocator> {
	typedef PIDeque<Type, Allocator> _CDeque;
	typedef deque<Type, Allocator> _stlc;
public:
	PIDeque() {piMonitor.containers++;}
	PIDeque(const Type & value) {piMonitor.containers++; _stlc::resize(1, value);}
	PIDeque(const Type & v0, const Type & v1) {piMonitor.containers++; _stlc::push_back(v0); _stlc::push_back(v1);}
	PIDeque(const Type & v0, const Type & v1, const Type & v2) {piMonitor.containers++; _stlc::push_back(v0); _stlc::push_back(v1); _stlc::push_back(v2);}
	PIDeque(const Type & v0, const Type & v1, const Type & v2, const Type & v3) {piMonitor.containers++; _stlc::push_back(v0); _stlc::push_back(v1); _stlc::push_back(v2); _stlc::push_back(v3);}
	~PIDeque() {piMonitor.containers--;}
	int size_s() const {return static_cast<int>(_stlc::size());}
	bool isEmpty() const {return _stlc::empty();}
	bool has(const Type & t) const {for (typename _stlc::const_iterator i = _stlc::begin(); i != _stlc::end(); ++i) if (t == *i) return true; return false;}
	int etries(const Type & t) const {int ec = 0; for (typename _stlc::const_iterator i = _stlc::begin(); i != _stlc::end(); ++i) if (t == *i) ++ec; return ec;}
	_CDeque & operator <<(const Type & t) {_CDeque::push_back(t); return *this;}
	PIDeque<Type> toVector() {PIDeque<Type> v; for (typename _stlc::const_iterator i = _stlc::begin(); i != _stlc::end(); ++i) v << *i; return v;}
};


#define __PIDEQUE_SIMPLE_FUNCTIONS__(T)


#endif


template<typename T>
inline std::ostream & operator <<(std::ostream & s, const PIDeque<T> & v) {s << "{"; for (size_t i = 0; i < v.size(); ++i) {s << v[i]; if (i < v.size() - 1) s << ", ";} s << "}"; return s;}

template<typename T>
inline PICout operator <<(PICout s, const PIDeque<T> & v) {s.space(); s.setControl(0, true); s << "{"; for (size_t i = 0; i < v.size(); ++i) {s << v[i]; if (i < v.size() - 1) s << ", ";} s << "}"; s.restoreControl(); return s;}


#endif // PIDEQUE_H
