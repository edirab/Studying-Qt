/*! \file pimap.h
 * \brief Associative array with custom types of key and value
 * 
 * This file declares PIMap
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

#ifndef PIMAP_H
#define PIMAP_H

#include "pivector.h"
#include "pideque.h"

class PIByteArray;

#if !defined(PIP_CONTAINERS_STL) || defined(DOXYGEN)

template<class T>
void piQuickSort(T * a, ssize_t N) {
	if (N < 1) return;
	ssize_t i = 0, j = N;
	T & p(a[N >> 1]);
	do {
		while (a[i] < p) i++;
		while (a[j] > p) j--;
		if (i <= j) {
			if (i != j) {
				//piCout << "swap" << i << j << a[i] << a[j];
				piSwapBinary<T>(a[i], a[j]);
			}
			i++; j--;
		}
	} while (i <= j);
	if (j > 0) piQuickSort(a, j);
	if (N > i) piQuickSort(a + i, N - i);
}

template <typename Key, typename T>
class PIMap {
	template <typename Key1, typename T1> friend PIByteArray & operator >>(PIByteArray & s, PIMap<Key1, T1> & v);
	template <typename Key1, typename T1> friend PIByteArray & operator <<(PIByteArray & s, const PIMap<Key1, T1> & v);
public:
	PIMap() {;}
	PIMap(const PIMap<Key, T> & other) {*this = other;}
	~PIMap() {;}

	PIMap<Key, T> & operator =(const PIMap<Key, T> & other) {
		if (this == &other) return *this;
		clear();
		pim_content = other.pim_content;
		pim_index = other.pim_index;
		return *this;
	}

	typedef T mapped_type;
	typedef Key key_type;
	typedef PIPair<Key, T> value_type;
	
	class iterator {
		friend class PIMap<Key, T>;
	private:
		iterator(const PIMap<Key, T> * v, ssize_t p): parent(v), pos(p) {}
		const PIMap<Key, T> * parent;
		ssize_t pos;
	public:
		iterator(): parent(0), pos(0) {}
		const Key & key() const {return const_cast<PIMap<Key, T> * >(parent)->_key(pos);}
		T & value() {return const_cast<PIMap<Key, T> * >(parent)->_value(pos);}
		void operator ++() {++pos;}
		void operator ++(int) {++pos;}
		void operator --() {--pos;}
		void operator --(int) {--pos;}
		bool operator ==(const iterator & it) const {return (pos == it.pos);}
		bool operator !=(const iterator & it) const {return (pos != it.pos);}
	};
	
	class reverse_iterator {
		friend class PIMap<Key, T>;
	private:
		reverse_iterator(const PIMap<Key, T> * v, ssize_t p): parent(v), pos(p) {}
		const PIMap<Key, T> * parent;
		ssize_t pos;
	public:
		reverse_iterator(): parent(0), pos(0) {}
		const Key & key() const {return const_cast<PIMap<Key, T> * >(parent)->_key(pos);}
		T & value() const {return const_cast<PIMap<Key, T> * >(parent)->_value(pos);}
		void operator ++() {--pos;}
		void operator ++(int) {--pos;}
		void operator --() {++pos;}
		void operator --(int) {++pos;}
		bool operator ==(const reverse_iterator & it) const {return (pos == it.pos);}
		bool operator !=(const reverse_iterator & it) const {return (pos != it.pos);}
	};
	
	class const_iterator {
		friend class PIMap<Key, T>;
	private:
		const_iterator(const PIMap<Key, T> * v, ssize_t p): parent(v), pos(p) {}
		const PIMap<Key, T> * parent;
		ssize_t pos;
	public:
		const_iterator(): parent(0), pos(0) {}
		const value_type operator *() const {return parent->_pair(pos);}
		const value_type* operator ->() const {cval = parent->_pair(pos); return &cval;}
		const Key & key() const {return const_cast<PIMap<Key, T> * >(parent)->_key(pos);}
		const T & value() const {return const_cast<PIMap<Key, T> * >(parent)->_value(pos);}
		void operator ++() {++pos;}
		void operator ++(int) {++pos;}
		void operator --() {--pos;}
		void operator --(int) {--pos;}
		bool operator ==(const const_iterator & it) const {return (pos == it.pos);}
		bool operator !=(const const_iterator & it) const {return (pos != it.pos);}
		mutable value_type cval;
	};
	
	class const_reverse_iterator {
		friend class PIMap<Key, T>;
	private:
		const_reverse_iterator(const PIMap<Key, T> * v, ssize_t p): parent(v), pos(p) {}
		const PIMap<Key, T> * parent;
		ssize_t pos;
	public:
		const_reverse_iterator(): parent(0), pos(0) {}
		const value_type operator *() const {return parent->_pair(pos);}
		const value_type* operator ->() const {cval = parent->_pair(pos); return &cval;}
		void operator ++() {--pos;}
		void operator ++(int) {--pos;}
		void operator --() {++pos;}
		void operator --(int) {++pos;}
		bool operator ==(const const_reverse_iterator & it) const {return (pos == it.pos);}
		bool operator !=(const const_reverse_iterator & it) const {return (pos != it.pos);}
		mutable value_type cval;
	};

 	iterator begin() {return iterator(this, 0);}
 	iterator end() {return iterator(this, size());}
	const_iterator begin() const {return const_iterator(this, 0);}
	const_iterator end() const {return const_iterator(this, size());}
 	reverse_iterator rbegin() {return reverse_iterator(this, size() - 1);}
 	reverse_iterator rend() {return reverse_iterator(this, -1);}
	const_reverse_iterator rbegin() const {return const_reverse_iterator(this, size() - 1);}
	const_reverse_iterator rend() const {return const_reverse_iterator(this, -1);}

	size_t size() const {return pim_content.size();}
	int size_s() const {return pim_content.size_s();}
	size_t length() const {return pim_content.size();}
	bool isEmpty() const {return (pim_content.size() == 0);}
	
	T & operator [](const Key & key) {
		bool f(false);
		ssize_t i = _find(key, f);
		if (f) return pim_content[pim_index[i].index];
		pim_content.push_back(T());
		pim_index.insert(i, MapIndex(key, pim_content.size() - 1));
		return pim_content.back();
	}
	const T operator [](const Key & key) const {bool f(false); ssize_t i = _find(key, f); if (f) return pim_content[pim_index[i].index]; return T();}
	T & at(const Key & key) {return (*this)[key];}
	const T at(const Key & key) const {return (*this)[key];}
	
	PIMap<Key, T> & operator <<(const PIMap<Key, T> & other) {
		if (other.isEmpty()) return *this;
		if (other.size() == 1) {insert(other.pim_index[0].key, other.pim_content[0]); return *this;}
		if (other.size() == 2) {insert(other.pim_index[0].key, other.pim_content[0]); insert(other.pim_index[1].key, other.pim_content[1]); return *this;}
		pim_content << other.pim_content;
		size_t si = pim_index.size();
		for (int i = 0; i < other.pim_index.size_s(); ++i)
			pim_index << MapIndex(other.pim_index[i].key, other.pim_index[i].index + si);
		_sort();
		return *this;
	}

	bool operator ==(const PIMap<Key, T> & t) const {return (pim_content == t.pim_content && pim_index == t.pim_index);}
	bool operator !=(const PIMap<Key, T> & t) const {return (pim_content != t.pim_content || pim_index != t.pim_index);}
	bool contains(const Key & key) const {bool f(false); _find(key, f); return f;}
	//int etries(const T & v) const {int ec = 0; for (size_t i = 0; i < pim_size; ++i) if (v == pim_data[i]) ++ec; return ec;}

	PIMap<Key, T> & reserve(size_t new_size) {pim_content.reserve(new_size); pim_index.reserve(new_size); return *this;}

	//PIMap<Key, T> & removeAll(const T & v) {for (llong i = 0; i < pim_size; ++i) if (pim_data[i] == v) {remove(i); --i;} return *this;}
	PIMap<Key, T> & removeOne(const Key & key) {bool f(false); ssize_t i = _find(key, f); if (f) _remove(i); return *this;}
	PIMap<Key, T> & remove(const Key & key) {return removeOne(key);}
	PIMap<Key, T> & erase(const Key & key) {return removeOne(key);}
	PIMap<Key, T> & clear() {pim_content.clear(); pim_index.clear(); return *this;}
	
	void swap(PIMap<Key, T> & other) {
		piSwapBinary<PIVector<T> >(pim_content, other.pim_content);
		piSwapBinary<PIVector<MapIndex> >(pim_index, other.pim_index);
	}

	PIMap<Key, T> & insert(const Key & key, const T & value) {
		//MapIndex * i = _find(key);
		bool f(false);
		ssize_t i = _find(key, f);
		//piCout << "insert key=" << key << "found=" << f << "index=" << i << "value=" << value;
		if (f) {
			pim_content[pim_index[i].index] = value;
		} else {
			pim_content.push_back(value);
			pim_index.insert(i, MapIndex(key, pim_content.size() - 1));
			//_sort();
		}
		return *this;
	}
	//const T value(const Key & key, const T & default_ = T()) const {MapIndex * i = _find(key); if (i == 0) return default_; return pim_content[i->index];}
	const T value(const Key & key, const T & default_ = T()) const {bool f(false); ssize_t i = _find(key, f); if (!f) return default_; return pim_content[pim_index[i].index];}
	PIVector<T> values() const {return pim_content;}
	Key key(const T & value_, const Key & default_ = Key()) const {for (int i = 0; i < pim_index.size_s(); ++i) if (pim_content[pim_index[i].index] == value_) return pim_index[i].key; return default_;}
	PIVector<Key> keys() const {
		PIVector<Key> ret;
		for (int i = 0; i < pim_index.size_s(); ++i)
			ret << pim_index[i].key;
		return ret;
	}
	
	void dump() {
		piCout << "PIMap" << size() << "entries" << PICoutManipulators::NewLine << "content:";
		for (size_t i = 0; i < pim_content.size(); ++i)
			piCout << PICoutManipulators::Tab << i << ":" << pim_content[i];
		piCout << "index:";
		for (size_t i = 0; i < pim_index.size(); ++i)
			piCout << PICoutManipulators::Tab << i << ":" << pim_index[i].key << "->" << pim_index[i].index;
	}
	
protected:
	struct MapIndex {
		MapIndex(Key k = Key(), size_t i = 0): key(k), index(i) {;}
		Key key;
		size_t index;
		bool operator ==(const MapIndex & s) const {return key == s.key;}
		bool operator !=(const MapIndex & s) const {return key != s.key;}
		bool operator <(const MapIndex & s) const {return key < s.key;}
		bool operator >(const MapIndex & s) const {return key > s.key;}
	};
	template <typename Key1, typename T1> friend PIByteArray & operator >>(PIByteArray & s, typename PIMap<Key1, T1>::MapIndex & v);
	template <typename Key1, typename T1> friend PIByteArray & operator <<(PIByteArray & s, const typename PIMap<Key1, T1>::MapIndex & v);

	ssize_t binarySearch(ssize_t first, ssize_t last, const Key & key, bool & found) const {
		ssize_t mid;
		while (first <= last) {
			mid = (first + last) / 2;
			if (key > pim_index[mid].key) first = mid + 1;
			else if (key < pim_index[mid].key) last = mid - 1;
			else {found = true; return mid;}
		}
		found = false;
		return first;
	}
	void _sort() {piQuickSort<MapIndex>(pim_index.data(), pim_index.size_s() - 1);}
	ssize_t _find(const Key & k, bool & found) const {
		/*for (size_t i = 0; i < pim_index.size(); ++i)
			if (pim_index[i].key == k) {
				return (MapIndex * )&(pim_index[i]);
			}
		return 0;*/
		//piCout << "find for" << k << pim_index.size_s();
		if (pim_index.isEmpty()) {
			found = false;
			return 0;
		}
		//piCout << k << ret << found;
		return binarySearch(0, pim_index.size_s() - 1, k, found);
	}
	void _remove(ssize_t index) {
		//if (index >= pim_index.size()) return;
		size_t ci = pim_index[index].index, bi = pim_index.size() - 1;
		pim_index.remove(index);
		for (size_t i = 0; i < pim_index.size(); ++i)
			if (pim_index[i].index == bi) {
				pim_index[i].index = ci;
				break;
			}
		piSwapBinary<T>(pim_content[ci], pim_content.back());
		pim_content.resize(pim_index.size());
	}
	const value_type _pair(ssize_t index) const {
		if (index < 0 || index >= pim_index.size_s())
			return value_type();
		//piCout << "_pair" << index << pim_index[index].index;
		return value_type(pim_index[index].key, pim_content[pim_index[index].index]);
	}
	Key & _key(ssize_t index) {return pim_index[index].key;}
	T & _value(ssize_t index) {return pim_content[pim_index[index].index];}
	
	PIVector<T> pim_content;
	PIDeque<MapIndex> pim_index;
};
//template <typename Key, typename T> bool operator <(const typename PIMap<Key, T>::MapIndex & f, const typename PIMap<Key, T>::MapIndex & s) {return f.key < s.key;}
//template <typename Key, typename T> bool operator >(const typename PIMap<Key, T>::MapIndex & f, const typename PIMap<Key, T>::MapIndex & s) {return f.key > s.key;}


/*#define __PIMAP_SIMPLE_FUNCTIONS__(T) 
	template<> inline PIMap<Key, T>::~PIMap() {dealloc(); _reset();} \
	template<> inline PIMap<Key, T> & PIMap<Key, T>::push_back(const T & v) {alloc(pim_size + 1); pim_data[pim_size - 1] = v; return *this;} \
	template<> inline PIMap<Key, T> & PIMap<Key, T>::fill(const T & f) { \
		for (size_t i = 0; i < pim_size; ++i) \
			pim_data[i] = f; \
		return *this; \
	} \
	template<> inline PIMap<Key, T> & PIMap<Key, T>::resize(size_t new_size, const T & f) { \
		if (new_size < pim_size) \
			pim_size = new_size; \
		if (new_size > pim_size) { \
			size_t os = pim_size; \
			alloc(new_size); \
			for (size_t i = os; i < new_size; ++i) pim_data[i] = f; \
		} \
		return *this; \
	} \
	template<> inline PIMap<Key, T> & PIMap<Key, T>::insert(size_t index, const T & v) { \
		alloc(pim_size + 1); \
		if (index < pim_size - 1) { \
			size_t os = pim_size - index - 1; \
			memmove(&(pim_data[index + 1]), &(pim_data[index]), os * sizeof(T)); \
		} \
		pim_data[index] = v; \
		return *this; \
	} \
	template<> inline PIMap<Key, T> & PIMap<Key, T>::remove(size_t index, size_t count) { \
		if (index + count >= pim_size) { \
			resize(index); \
			return *this; \
		} \
		size_t os = pim_size - index - count; \
		memmove(&(pim_data[index]), &(pim_data[index + count]), os * sizeof(T)); \
		pim_size -= count; \
		return *this; \
	}

__PIMAP_SIMPLE_FUNCTIONS__(char)
__PIMAP_SIMPLE_FUNCTIONS__(uchar)
__PIMAP_SIMPLE_FUNCTIONS__(short)
__PIMAP_SIMPLE_FUNCTIONS__(ushort)
__PIMAP_SIMPLE_FUNCTIONS__(int)
__PIMAP_SIMPLE_FUNCTIONS__(uint)
__PIMAP_SIMPLE_FUNCTIONS__(long)
__PIMAP_SIMPLE_FUNCTIONS__(ulong)
__PIMAP_SIMPLE_FUNCTIONS__(llong)
__PIMAP_SIMPLE_FUNCTIONS__(ullong)
__PIMAP_SIMPLE_FUNCTIONS__(float)
__PIMAP_SIMPLE_FUNCTIONS__(double)
__PIMAP_SIMPLE_FUNCTIONS__(ldouble)*/

#else


template<typename Key, typename Type>
class PIP_EXPORT PIMap: public map<Key, Type> {
	typedef PIMap<Key, Type> _CMap;
	typedef map<Key, Type> _stlc;
	typedef std::pair<Key, Type> _stlpair;
public:
	PIMap() {;}
	PIMap(const Key & key_, const Type & value_) {insert(key_, value_);}
	bool isEmpty() const {return _stlc::empty();}
	bool contains(const Key & key_) const {return _stlc::count(key_) > 0;}
	int size_s() const {return static_cast<int>(_stlc::size());}
	_CMap & insert(const Key & key_, const Type & value_) {_stlc::insert(_stlpair(key_, value_)); return *this;}
	_CMap & insert(PIPair<Key, Type> entry_) {_stlc::insert(_stlpair(entry_.first, entry_.second)); return *this;}
	Key key(Type value_, const Key & default_ = Key()) const {for (typename _stlc::const_iterator i = _stlc::begin(); i != _stlc::end(); i++) if (i->second == value_) return i->first; return default_;}
	PIVector<Key> keys() const {
		PIVector<Key> ret;
		for (typename _stlc::const_iterator i = _stlc::begin(); i != _stlc::end(); i++)
			ret << i->first;
		return ret;
	}
	Type & at(const Key & key_) {return _stlc::find(key_)->second;}
	Type value(const Key & key_) const {typename _stlc::const_iterator it = _stlc::find(key_); if (it != _stlc::end()) return it->second; return Type();}
};


template<typename Key, typename Type>
class PIP_EXPORT PIMultiMap: public multimap<Key, Type> {
	typedef PIMultiMap<Key, Type> _CMultiMap;
	typedef multimap<Key, Type> _stlc;
	typedef std::pair<Key, Type> _stlpair;
public:
	PIMultiMap() {;}
	PIMultiMap(const Key & key_, const Type & value_) {insert(key_, value_);}
	_CMultiMap & insert(const Key & key_, const Type & value_) {_stlc::insert(_stlpair(key_, value_)); return *this;}
	_CMultiMap & insert(PIPair<Key, Type> entry_) {_stlc::insert(_stlpair(entry_.first, entry_.second)); return *this;}
	bool isEmpty() const {return _stlc::empty();}
	bool contains(const Key & key_) const {return _stlc::count(key_) > 0;}
	Key key(Type value_, const Key & default_ = Key()) const {for (typename _stlc::const_iterator i = _stlc::begin(); i != _stlc::end(); i++) if (i->second == value_) return i->first; return default_;}
	PIVector<Key> keys(Type value_) const {
		PIVector<Key> ret;
		for (typename _stlc::const_iterator i = _stlc::begin(); i != _stlc::end(); i++)
			if (i->second == value_)
				ret << i->first;
		return ret;
	}
	Type & value(const Key & key_) {typename _stlc::iterator i = _stlc::find(key_); if (i == _stlc::end()) return Type(); return i->second;}
	Type value(const Key & key_) const {typename _stlc::const_iterator i = _stlc::find(key_); if (i == _stlc::end()) return Type(); return i->second;}
	PIVector<Type> values(const Key & key_) const {
		std::pair<typename _stlc::const_iterator, typename _stlc::const_iterator> range = _stlc::equal_range(key_);
		PIVector<Type> ret;
		for (typename _stlc::const_iterator i = range.first; i != range.second; ++i)
			ret << i->second;
		return ret;
	}
	Type & operator [](const Key & key_) {if (!contains(key_)) return _stlc::insert(_stlpair(key_, Type()))->second; return _stlc::find(key_)->second;}
	Type operator [](const Key & key_) const {return _stlc::find(key_)->second;}
};

#define __PIMAP_SIMPLE_FUNCTIONS__(T)


#endif


template<typename Key, typename Type>
inline std::ostream & operator <<(std::ostream & s, const PIMap<Key, Type> & v) {
	s << "{";
	bool first = true;
	for (typename PIMap<Key, Type>::const_iterator i = v.begin(); i != v.end(); ++i) {
		if (!first)
			s << ", ";
		first = false;
		s << i->first << ": " << i->second;
	}
	s << "}";
	return s;
}

template<typename Key, typename Type>
inline PICout operator <<(PICout s, const PIMap<Key, Type> & v) {
	s.space();
	s.setControl(0, true);
	s << "{";
	bool first = true;
	for (typename PIMap<Key, Type>::const_iterator i = v.begin(); i != v.end(); ++i) {
		if (!first)
			s << ", ";
		first = false;
		s << i->first << ": " << i->second;
	}
	s << "}";
	s.restoreControl();
	return s;
}


#endif // PIMAP_H
