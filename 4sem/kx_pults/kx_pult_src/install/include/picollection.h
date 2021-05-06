/*! \file picollection.h
 * \brief Custom elements collection
*/
/*
    PIP - Platform Independent Primitives
    Peer - named I/O ethernet node, forming self-organized peering network
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

#ifndef PICOLLECTION_H
#define PICOLLECTION_H

#include "piobject.h"

#ifdef DOXYGEN

/** \brief Add existing element "object" in group with name "group"
 * \relatesalso PICollection
 * \details If there is no group with name "group" it will be created.
 * Only one element of the class "object" can be in group "group". If
 * this is already exists nothing be happens. \n "object" should to
 * be pointer to object based on PIObject. */
#  define ADD_TO_COLLECTION(group, object)

/** \brief Add new element of class "class" in group with name "group"
 * \relatesalso PICollection
 * \details If there is no group with name "group" it will be created.
 * Only one element of the class "class" can be in group "group". If
 * this is already exists nothing be happens. \n "class" should to
 * be name of the any class based on PIObject. */
#  define ADD_NEW_TO_COLLECTION(group, class)

#else
#  define ADD_TO_COLLECTION(group, object) static PICollection::CollectionAdder __##group##_##__LINE__##_##adder##__(#group, object);
#  define ADD_TO_COLLECTION_WITH_NAME(group, object, name) static PICollection::CollectionAdder __##group##_##__LINE__##_##adder##__(#group, object, #name);
#  define ADD_NEW_TO_COLLECTION(group, class) static PICollection::CollectionAdder __##group##_##class##_##adder##__(#group, new class());
#  define ADD_NEW_TO_COLLECTION_WITH_NAME(group, class, name) static PICollection::CollectionAdder __##group##_##class##_##adder##__(#group, new class(), #name);
#endif

class PIP_EXPORT PICollection
{
	friend class __PICollectionInitializer;
public:
	PICollection() {;}
	
	//! \brief Returns all existing groups by their names
	static PIStringList groups();
	
	//! \brief Returns all elements of group "group"
	static PIVector<const PIObject * > groupElements(const PIString & group);
	
	static void addToGroup(const PIString & group, const PIObject * element);

	class CollectionAdder {
	public:
		CollectionAdder(const PIString & group, const PIObject * element, const PIString & name = PIString());
	};
	
protected:
	struct Group {
		Group(const PIString & name_ = PIString()) {name = name_;}
		//~Group() {piCout << "delete group" << name << this; piForeach (const PIObject * o, elements) delete o; elements.clear();}
		PIString name;
		PIVector<const PIObject * > elements;
	};
	
	static PIVector<Group> * _groups;
	
};

class PIP_EXPORT __PICollectionInitializer {
public:
	__PICollectionInitializer() {
		if (_inited_) return;
		_inited_ = true;
		PICollection::_groups = new PIVector<PICollection::Group>();
	}
	static bool _inited_;
};

static __PICollectionInitializer __picollectioninitializer;

#endif // PICOLLECTION_H
