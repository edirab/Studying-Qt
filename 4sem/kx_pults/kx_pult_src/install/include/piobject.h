/*! \file piobject.h
 * \brief Base object
 * 
 * This file declare PIObject class and associated macros
*/
/*
    PIP - Platform Independent Primitives
    Object, base class of some PIP classes, provide EVENT -> EVENT_HANDLER mechanism
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

#ifndef PIOBJECT_H
#define PIOBJECT_H

#include "piinit.h"
#include "pivariant.h"
#include "pimutex.h"


#ifdef DOXYGEN


/// \relatesalso PIObject \brief you should use this macro after class declaration to use EVENT and EVENT_HANDLER and correct piCoutObj output
#define PIOBJECT(name)

/// \relatesalso PIObject \brief you should use this macro after class declaration to use EVENT and EVENT_HANDLER of parent class
#define PIOBJECT_SUBCLASS(name, parent)


/// \relatesalso PIObject \brief declare event handler \"event\" with name \"name\" and return type \"ret\", ret name()
#define EVENT_HANDLER0(ret, name) ret name()

/// \relatesalso PIObject \brief declare event handler \"event\" with name \"name\" and return type \"ret\", ret name(type0 var0)
#define EVENT_HANDLER1(ret, name, type0, var0) ret name(type0 var0)

/// \relatesalso PIObject \brief declare event handler \"event\" with name \"name\" and return type \"ret\", ret name(type0 var0, type1 var1)
#define EVENT_HANDLER2(ret, name, type0, var0, type1, var1) ret name(type0 var0, type1 var1)

/// \relatesalso PIObject \brief declare event handler \"event\" with name \"name\" and return type \"ret\", ret name(type0 var0, type1 var1, type2 var2)
#define EVENT_HANDLER3(ret, name, type0, var0, type1, var1, type2, var2) ret name(type0 var0, type1 var1, type2 var2)

/// \relatesalso PIObject \brief declare event handler \"event\" with name \"name\" and return type \"ret\", ret name(type0 var0, type1 var1, type2 var2, type3 var3)
#define EVENT_HANDLER4(ret, name, type0, var0, type1, var1, type2, var2, type3, var3) ret name(type0 var0, type1 var1, type2 var2, type3 var3)

/// \relatesalso PIObject \brief EVENT_HANDLER is synonym of EVENT_HANDLER0
#define EVENT_HANDLER EVENT_HANDLER0


/// \relatesalso PIObject \brief declare virtual event handler \"event\" with name \"name\" and return type \"ret\", virtual ret name()
#define EVENT_VHANDLER0(ret, name) virtual ret name()

/// \relatesalso PIObject \brief declare virtual event handler \"event\" with name \"name\" and return type \"ret\", virtual ret name(type0 var0)
#define EVENT_VHANDLER1(ret, name, type0, var0) virtual ret name(type0 var0)

/// \relatesalso PIObject \brief declare virtual event handler \"event\" with name \"name\" and return type \"ret\", virtual ret name(type0 var0, type1 var1)
#define EVENT_VHANDLER2(ret, name, type0, var0, type1, var1) virtual ret name(type0 var0, type1 var1)

/// \relatesalso PIObject \brief declare virtual event handler \"event\" with name \"name\" and return type \"ret\", virtual ret name(type0 var0, type1 var1, type2 var2)
#define EVENT_VHANDLER3(ret, name, type0, var0, type1, var1, type2, var2) virtual ret name(type0 var0, type1 var1, type2 var2)

/// \relatesalso PIObject \brief declare virtual event handler \"event\" with name \"name\" and return type \"ret\", virtual ret name(type0 var0, type1 var1, type2 var2, type3 var3)
#define EVENT_VHANDLER4(ret, name, type0, var0, type1, var1, type2, var2, type3, var3) virtual ret name(type0 var0, type1 var1, type2 var2, type3 var3)

/// \relatesalso PIObject \brief EVENT_VHANDLER is synonym of EVENT_VHANDLER0
#define EVENT_VHANDLER EVENT_VHANDLER0


/// \relatesalso PIObject \brief declare event \"event\" with name \"name\", void name();
#define EVENT0(name) void name();

/// \relatesalso PIObject \brief declare event \"event\" with name \"name\", void name(type0 var0);
#define EVENT1(name, type0, var0) void name(type0 var0);

/// \relatesalso PIObject \brief declare event \"event\" with name \"name\", void name(type0 var0, type1 var1);
#define EVENT2(name, type0, var0, type1, var1) void name(type0 var0, type1 var1);

/// \relatesalso PIObject \brief declare event \"event\" with name \"name\", void name(type0 var0, type1 var1, type2 var2);
#define EVENT3(name, type0, var0, type1, var1, type2, var2) void name(type0 var0, type1 var1, type2 var2);

/// \relatesalso PIObject \brief declare event \"event\" with name \"name\", void name(type0 var0, type1 var1, type2 var2, type3 var3);
#define EVENT4(name, type0, var0, type1, var1, type2, var2, type3, var3) void name(type0 var0, type1 var1, type2 var2, type3 var3);

/// \relatesalso PIObject \brief EVENT is synonym of EVENT0
#define EVENT EVENT0


#define RAISE_EVENT0(src, event)
#define RAISE_EVENT1(src, event, v0)
#define RAISE_EVENT2(src, event, v0, v1)
#define RAISE_EVENT3(src, event, v0, v1, v2)
#define RAISE_EVENT4(src, event, v0, v1, v2, v3)
#define RAISE_EVENT RAISE_EVENT0


/// \relatesalso PIObject \brief connect event \"event\" from object \"src\"  to  event handler \"handler\". \"Event\" and \"handler\" must has equal argument lists.
#define CONNECTU(src, event, dest, handler)

/// \relatesalso PIObject \brief connect event \"event\" from object \"src\"  to  event handler \"handler\" with return type \"ret\" from object \"dest\" with check of event and handler exists
#define CONNECT0(ret, src, event, dest, handler)

/// \relatesalso PIObject \brief connect event \"event\" from object \"src\"  to  event handler \"handler\" with return type \"ret\" from object \"dest\" with check of event and handler exists
#define CONNECT1(ret, type0, src, event, dest, handler)

/// \relatesalso PIObject \brief connect event \"event\" from object \"src\"  to  event handler \"handler\" with return type \"ret\" from object \"dest\" with check of event and handler exists
#define CONNECT2(ret, type0, type1, src, event, dest, handler)

/// \relatesalso PIObject \brief connect event \"event\" from object \"src\"  to  event handler \"handler\" with return type \"ret\" from object \"dest\" with check of event and handler exists
#define CONNECT3(ret, type0, type1, type2, src, event, dest, handler)

/// \relatesalso PIObject \brief connect event \"event\" from object \"src\"  to  event handler \"handler\" with return type \"ret\" from object \"dest\" with check of event and handler exists
#define CONNECT4(ret, type0, type1, type2, type3, src, event, dest, handler)

/// \relatesalso PIObject \brief CONNECT is synonym of CONNECT0
#define CONNECT CONNECT0


/// \relatesalso PIObject \brief connect event \"event\" from object \"src\"  to  event handler \"handler\" with return type \"ret\" from object \"dest\" without check of event exists
#define WEAK_CONNECT0(ret, src, event, dest, handler)

/// \relatesalso PIObject \brief connect event \"event\" from object \"src\"  to  event handler \"handler\" with return type \"ret\" from object \"dest\" without check of event exists
#define WEAK_CONNECT1(ret, type0, src, event, dest, handler)

/// \relatesalso PIObject \brief connect event \"event\" from object \"src\"  to  event handler \"handler\" with return type \"ret\" from object \"dest\" without check of event exists
#define WEAK_CONNECT2(ret, type0, type1, src, event, dest, handler)

/// \relatesalso PIObject \brief connect event \"event\" from object \"src\"  to  event handler \"handler\" with return type \"ret\" from object \"dest\" without check of event exists
#define WEAK_CONNECT3(ret, type0, type1, type2, src, event, dest, handler)

/// \relatesalso PIObject \brief connect event \"event\" from object \"src\"  to  event handler \"handler\" with return type \"ret\" from object \"dest\" without check of event exists
#define WEAK_CONNECT4(ret, type0, type1, type2, type3, src, event, dest, handler)

/// \relatesalso PIObject \brief WEAK_CONNECT is synonym of WEAK_CONNECT0
#define WEAK_CONNECT WEAK_CONNECT0


/// \relatesalso PIObject \brief piDisconnect event \"event\" from object \"src\"  from  event handler \"handler\" with return type \"ret\" from object \"dest\"
#define DISCONNECT0(ret, src, event, dest, handler)

/// \relatesalso PIObject \brief piDisconnect event \"event\" from object \"src\"  from  event handler \"handler\" with return type \"ret\" from object \"dest\"
#define DISCONNECT1(ret, type0, src, event, dest, handler)

/// \relatesalso PIObject \brief piDisconnect event \"event\" from object \"src\"  from  event handler \"handler\" with return type \"ret\" from object \"dest\"
#define DISCONNECT2(ret, type0, type1, src, event, dest, handler)

/// \relatesalso PIObject \brief piDisconnect event \"event\" from object \"src\"  from  event handler \"handler\" with return type \"ret\" from object \"dest\"
#define DISCONNECT3(ret, type0, type1, type2, src, event, dest, handler)

/// \relatesalso PIObject \brief piDisconnect event \"event\" from object \"src\"  from  event handler \"handler\" with return type \"ret\" from object \"dest\"
#define DISCONNECT4(ret, type0, type1, type2, type3, src, event, dest, handler)

/// \relatesalso PIObject \brief DISCONNECT is synonym of DISCONNECT0
#define DISCONNECT DISCONNECT0


/// \relatesalso PIObject \brief Returns pointer to events handler \"handler\"
#define HANDLER(handler)


#define PIOBJECT(name)
#define PIOBJECT_SUBCLASS(name)


#else


#define _PI_STR(x) #x
#define _PI_SSTR(x) _PI_STR(x)
#define LOCATION __FILE__ ":" _PI_SSTR(__LINE__)


#define PIOBJECT(name) \
	protected: \
	typedef name __PIObject__; \
	static const PIString __classNameS() {return PIStringAscii(#name);} \
	public: \
	virtual const char * className() const {return #name;} \
	private:

#define PIOBJECT_PARENT(name) \
	class __##name##_ParentInitializer__ { \
	public: \
		__##name##_ParentInitializer__() { \
			PIString pn(name::__classNameS()); \
			if (pn.isEmpty()) return; \
			PIMutexLocker ml(__eh_mutex); \
			if (__eh_data.contains(__classNameS())) return; \
			__eh_data[pn]; \
			__eh_data[__classNameS()]; \
			__EHData & ehp(__eh_data[pn]); \
			__EHData & eh(__eh_data[__classNameS()]); \
			eh.eh_set << ehp.eh_set; \
			eh.eh_func << ehp.eh_func; \
		} \
	}; \
	__##name##_ParentInitializer__ __##name##_parent_init__; \
	public: \
	virtual const char * superClassName() const {return #name;} \
	private:

#define PIOBJECT_SUBCLASS(name, parent) PIOBJECT(name) PIOBJECT_PARENT(parent)


#define EH_INIT0(ret, name) \
	class __##name##0_Initializer__ { \
	public: \
		__##name##0_Initializer__() { \
			PIMutexLocker ml(__eh_mutex); \
			__EHData & eh(__eh_data[__classNameS()]); \
			void * fp = (void*)(ret(*)(void*))__stat_eh_##name##__; \
			if (eh.eh_set[fp]) return; \
			eh.eh_set << fp; \
			__EHFunc & f(eh.eh_func[fp]); \
			f.scope = __classNameS(); \
			f.func_name = PIStringAscii(#name); \
			f.addr = fp; \
			f.type_ret = PIStringAscii(#ret); \
		} \
	}; \
	__##name##0_Initializer__ __##name##0_init__; \

#define EH_INIT1(ret, name, a0, n0) \
	class __##name##1##n0##_Initializer__ { \
	public: \
		__##name##1##n0##_Initializer__() { \
			PIMutexLocker ml(__eh_mutex); \
			__EHData & eh(__eh_data[__classNameS()]); \
			void * fp = (void*)(ret(*)(void*, a0))__stat_eh_##name##__; \
			if (eh.eh_set[fp]) return; \
			eh.eh_set << fp; \
			__EHFunc & f(eh.eh_func[fp]); \
			f.scope = __classNameS(); \
			f.func_name = PIStringAscii(#name); \
			f.addr = fp; \
			f.type_ret = PIStringAscii(#ret); \
			f.types << PIObject::simplifyType(#a0); \
			f.names << PIStringAscii(#n0); \
		} \
	}; \
	__##name##1##n0##_Initializer__ __##name##1##n0##_init__; \

#define EH_INIT2(ret, name, a0, n0, a1, n1) \
	class __##name##2##n0##n1##_Initializer__ { \
	public: \
		__##name##2##n0##n1##_Initializer__() { \
			PIMutexLocker ml(__eh_mutex); \
			__EHData & eh(__eh_data[__classNameS()]); \
			void * fp = (void*)(ret(*)(void*, a0, a1))__stat_eh_##name##__; \
			if (eh.eh_set[fp]) return; \
			eh.eh_set << fp; \
			__EHFunc & f(eh.eh_func[fp]); \
			f.scope = __classNameS(); \
			f.func_name = PIStringAscii(#name); \
			f.addr = fp; \
			f.type_ret = PIStringAscii(#ret); \
			f.types << PIObject::simplifyType(#a0) << PIObject::simplifyType(#a1); \
			f.names << PIStringAscii(#n0) << PIStringAscii(#n1); \
		} \
	}; \
	__##name##2##n0##n1##_Initializer__ __##name##2##n0##n1##_init__; \

#define EH_INIT3(ret, name, a0, n0, a1, n1, a2, n2) \
	class __##name##3##n0##n1##n2##_Initializer__ { \
	public: \
		__##name##3##n0##n1##n2##_Initializer__() { \
			PIMutexLocker ml(__eh_mutex); \
			__EHData & eh(__eh_data[__classNameS()]); \
			void * fp = (void*)(ret(*)(void*, a0, a1, a2))__stat_eh_##name##__; \
			if (eh.eh_set[fp]) return; \
			eh.eh_set << fp; \
			__EHFunc & f(eh.eh_func[fp]); \
			f.scope = __classNameS(); \
			f.func_name = PIStringAscii(#name); \
			f.addr = fp; \
			f.type_ret = PIStringAscii(#ret); \
			f.types << PIObject::simplifyType(#a0) << PIObject::simplifyType(#a1) << PIObject::simplifyType(#a2); \
			f.names << PIStringAscii(#n0) << PIStringAscii(#n1) << PIStringAscii(#n2); \
		} \
	}; \
	__##name##3##n0##n1##n2##_Initializer__ __##name##3##n0##n1##n2##_init__; \

#define EH_INIT4(ret, name, a0, n0, a1, n1, a2, n2, a3, n3) \
	class __##name##4##n0##n1##n2##n3##_Initializer__ { \
	public: \
		__##name##4##n0##n1##n2##n3##_Initializer__() { \
			PIMutexLocker ml(__eh_mutex); \
			__EHData & eh(__eh_data[__classNameS()]); \
			void * fp = (void*)(ret(*)(void*, a0, a1, a2, a3))__stat_eh_##name##__; \
			if (eh.eh_set[fp]) return; \
			eh.eh_set << fp; \
			__EHFunc & f(eh.eh_func[fp]); \
			f.scope = __classNameS(); \
			f.func_name = PIStringAscii(#name); \
			f.addr = fp; \
			f.type_ret = PIStringAscii(#ret); \
			f.types << PIObject::simplifyType(#a0) << PIObject::simplifyType(#a1) << PIObject::simplifyType(#a2) << PIObject::simplifyType(#a3); \
			f.names << PIStringAscii(#n0) << PIStringAscii(#n1) << PIStringAscii(#n2) << PIStringAscii(#n3); \
		} \
	}; \
	__##name##4##n0##n1##n2##n3##_Initializer__ __##name##4##n0##n1##n2##n3##_init__; \


#define EVENT_HANDLER0(ret, name) \
	EH_INIT0(ret, name) \
	static ret __stat_eh_##name##__(void * __o__) {return ((__PIObject__*)__o__)->name();} \
	ret name()

#define EVENT_HANDLER1(ret, name, a0, n0) \
	EH_INIT1(ret, name, a0, n0) \
	static ret __stat_eh_##name##__(void * __o__, a0 n0) {return ((__PIObject__*)__o__)->name(n0);} \
	ret name(a0 n0)

#define EVENT_HANDLER2(ret, name, a0, n0, a1, n1) \
	EH_INIT2(ret, name, a0, n0, a1, n1) \
	static ret __stat_eh_##name##__(void * __o__, a0 n0, a1 n1) {return ((__PIObject__*)__o__)->name(n0, n1);} \
	ret name(a0 n0, a1 n1)

#define EVENT_HANDLER3(ret, name, a0, n0, a1, n1, a2, n2) \
	EH_INIT3(ret, name, a0, n0, a1, n1, a2, n2) \
	static ret __stat_eh_##name##__(void * __o__, a0 n0, a1 n1, a2 n2) {return ((__PIObject__*)__o__)->name(n0, n1, n2);} \
	ret name(a0 n0, a1 n1, a2 n2)

#define EVENT_HANDLER4(ret, name, a0, n0, a1, n1, a2, n2, a3, n3) \
	EH_INIT4(ret, name, a0, n0, a1, n1, a2, n2, a3, n3) \
	static ret __stat_eh_##name##__(void * __o__, a0 n0, a1 n1, a2 n2, a3 n3) {return ((__PIObject__*)__o__)->name(n0, n1, n2, n3);} \
	ret name(a0 n0, a1 n1, a2 n2, a3 n3)

#define EVENT_HANDLER EVENT_HANDLER0


#define EVENT_VHANDLER0(ret, name) \
	EH_INIT0(ret, name) \
	static ret __stat_eh_##name##__(void * __o__) {return ((__PIObject__*)__o__)->name();} \
	virtual ret name()

#define EVENT_VHANDLER1(ret, name, a0, n0) \
	EH_INIT1(ret, name, a0, n0) \
	static ret __stat_eh_##name##__(void * __o__, a0 n0) {return ((__PIObject__*)__o__)->name(n0);} \
	virtual ret name(a0 n0)

#define EVENT_VHANDLER2(ret, name, a0, n0, a1, n1) \
	EH_INIT2(ret, name, a0, n0, a1, n1) \
	static ret __stat_eh_##name##__(void * __o__, a0 n0, a1 n1) {return ((__PIObject__*)__o__)->name(n0, n1);} \
	virtual ret name(a0 n0, a1 n1)

#define EVENT_VHANDLER3(ret, name, a0, n0, a1, n1, a2, n2) \
	EH_INIT3(ret, name, a0, n0, a1, n1, a2, n2) \
	static ret __stat_eh_##name##__(void * __o__, a0 n0, a1 n1, a2 n2) {return ((__PIObject__*)__o__)->name(n0, n1, n2);} \
	virtual ret name(a0 n0, a1 n1, a2 n2)

#define EVENT_VHANDLER4(ret, name, a0, n0, a1, n1, a2, n2, a3, n3) \
	EH_INIT4(ret, name, a0, n0, a1, n1, a2, n2, a3, n3) \
	static ret __stat_eh_##name##__(void * __o__, a0 n0, a1 n1, a2 n2, a3 n3) {return ((__PIObject__*)__o__)->name(n0, n1, n2, n3);} \
	virtual ret name(a0 n0, a1 n1, a2 n2, a3 n3)

#define EVENT_VHANDLER EVENT_VHANDLER0


#define EVENT0(name) EVENT_HANDLER0(void, name) {PIObject::raiseEvent(this, PIStringAscii(#name));}
#define EVENT1(name, a0, n0) EVENT_HANDLER1(void, name, a0, n0) {PIObject::raiseEvent(this, PIStringAscii(#name), n0);}
#define EVENT2(name, a0, n0, a1, n1) EVENT_HANDLER2(void, name, a0, n0, a1, n1) {PIObject::raiseEvent(this, PIStringAscii(#name), n0, n1);}
#define EVENT3(name, a0, n0, a1, n1, a2, n2) EVENT_HANDLER3(void, name, a0, n0, a1, n1, a2, n2) {PIObject::raiseEvent(this, PIStringAscii(#name), n0, n1, n2);}
#define EVENT4(name, a0, n0, a1, n1, a2, n2, a3, n3) EVENT_HANDLER4(void, name, a0, n0, a1, n1, a2, n2, a3, n3) {PIObject::raiseEvent(this, PIStringAscii(#name), n0, n1, n2, n3);}
#define EVENT EVENT0

#define RAISE_EVENT0(src, event) (src)->event();
#define RAISE_EVENT1(src, event, v0) (src)->event(v0);
#define RAISE_EVENT2(src, event, v0, v1) (src)->event(v0, v1);
#define RAISE_EVENT3(src, event, v0, v1, v2) (src)->event(v0, v1, v2);
#define RAISE_EVENT4(src, event, v0, v1, v2, v3) (src)->event(v0, v1, v2, v3);
#define RAISE_EVENT RAISE_EVENT0

#define CONNECTU(src, event, dest, handler) PIObject::piConnectU(src, PIStringAscii(#event), dest, dest, PIStringAscii(#handler), LOCATION);

#define CONNECT0(ret, src, event, dest, handler) PIObject::piConnect(src, PIStringAscii(#event), dest, dest, (void*)(ret(*)(void*))(&(dest)->__stat_eh_##handler##__), (void*)(void(*)(void*))(&(src)->__stat_eh_##event##__), 0, LOCATION);
#define CONNECT1(ret, a0, src, event, dest, handler) PIObject::piConnect(src, PIStringAscii(#event), dest, dest, (void*)(ret(*)(void*, a0))(&(dest)->__stat_eh_##handler##__), (void*)(void(*)(void*, a0))(&(src)->__stat_eh_##event##__), 1, LOCATION);
#define CONNECT2(ret, a0, a1, src, event, dest, handler) PIObject::piConnect(src, PIStringAscii(#event), dest, dest, (void*)(ret(*)(void*, a0, a1))(&(dest)->__stat_eh_##handler##__), (void*)(void(*)(void*, a0, a1))(&(src)->__stat_eh_##event##__), 2, LOCATION);
#define CONNECT3(ret, a0, a1, a2, src, event, dest, handler) PIObject::piConnect(src, PIStringAscii(#event), dest, dest, (void*)(ret(*)(void*, a0, a1, a2))(&(dest)->__stat_eh_##handler##__), (void*)(void(*)(void*, a0, a1, a2))(&(src)->__stat_eh_##event##__), 3, LOCATION);
#define CONNECT4(ret, a0, a1, a2, a3, src, event, dest, handler) PIObject::piConnect(src, PIStringAscii(#event), dest, dest, (void*)(ret(*)(void*, a0, a1, a2, a3))(&(dest)->__stat_eh_##handler##__), (void*)(void(*)(void*, a0, a1, a2, a3))(&(src)->__stat_eh_##event##__), 4, LOCATION);
#define CONNECT CONNECT0

#define WEAK_CONNECT0(ret, src, event, dest, handler) PIObject::piConnect(src, PIStringAscii(#event), dest, dest, (void*)(ret(*)(void*))(&(dest)->__stat_eh_##handler##__), 0, 0, LOCATION);
#define WEAK_CONNECT1(ret, a0, src, event, dest, handler) PIObject::piConnect(src, PIStringAscii(#event), dest, dest, (void*)(ret(*)(void*, a0))(&(dest)->__stat_eh_##handler##__), 0, 1, LOCATION);
#define WEAK_CONNECT2(ret, a0, a1, src, event, dest, handler) PIObject::piConnect(src, PIStringAscii(#event), dest, dest, (void*)(ret(*)(void*, a0, a1))(&(dest)->__stat_eh_##handler##__), 0, 2, LOCATION);
#define WEAK_CONNECT3(ret, a0, a1, a2, src, event, dest, handler) PIObject::piConnect(src, PIStringAscii(#event), dest, dest, (void*)(ret(*)(void*, a0, a1, a2))(&(dest)->__stat_eh_##handler##__), 0, 3, LOCATION);
#define WEAK_CONNECT4(ret, a0, a1, a2, a3, src, event, dest, handler) PIObject::piConnect(src, PIStringAscii(#event), dest, dest, (void*)(ret(*)(void*, a0, a1, a2, a3))(&(dest)->__stat_eh_##handler##__), 0, 4, LOCATION);
#define WEAK_CONNECT WEAK_CONNECT0

#define DISCONNECT0(ret, src, event, dest, handler) PIObject::piDisconnect(src, PIStringAscii(#event), dest, (void*)(ret(*)(void*))(&(dest)->__stat_eh_##handler##__));
#define DISCONNECT1(ret, a0, src, event, dest, handler) PIObject::piDisconnect(src, PIStringAscii(#event), dest, (void*)(ret(*)(void*, a0))(&(dest)->__stat_eh_##handler##__));
#define DISCONNECT2(ret, a0, a1, src, event, dest, handler) PIObject::piDisconnect(src, PIStringAscii(#event), dest, (void*)(ret(*)(void*, a0, a1))(&(dest)->__stat_eh_##handler##__));
#define DISCONNECT3(ret, a0, a1, a2, src, event, dest, handler) PIObject::piDisconnect(src, PIStringAscii(#event), dest, (void*)(ret(*)(void*, a0, a1, a2))(&(dest)->__stat_eh_##handler##__));
#define DISCONNECT4(ret, a0, a1, a2, a3, src, event, dest, handler) PIObject::piDisconnect(src, PIStringAscii(#event), dest, (void*)(ret(*)(void*, a0, a1, a2, a3))(&(dest)->__stat_eh_##handler##__));
#define DISCONNECT DISCONNECT0

#define HANDLER(handler) __stat_eh_##handler##__

#define __PIOBJECT_SIGNATURE__ 0xabcdbadc

#endif

typedef void (*Handler)(void * );

class PIP_EXPORT PIObject
{
	friend class PIObjectManager;
	friend void dumpApplication();
	typedef PIObject __PIObject__;
public:
	
	//! Contructs PIObject with name "name"
	PIObject(const PIString & name = PIString());
	
	virtual ~PIObject();
	
private:
	uint _signature_;
	
public:
	
	
	//! Returns object name
	PIString name() const {return property(PIStringAscii("name")).toString();}
	
	//! Returns object class name
	virtual const char * className() const {return "PIObject";}
	
	//! Returns object superclass name
	virtual const char * superClassName() const {return "";}
	
	//! Return if debug of this object is active
	bool debug() const {return property(PIStringAscii("debug")).toBool();}
	
	
	//! Set object name
	void setName(const PIString & name) {setProperty(PIStringAscii("name"), name);}
	void setName(const char * name) {setName(PIStringAscii(name));}
	
	//! Set object debug active
	void setDebug(bool debug) {setProperty(PIStringAscii("debug"), debug);}
	
	//! Returns properties of the object
	const PIMap<PIString, PIVariant> & properties() const {return properties_;}
	
	//! Returns properties count of the object
	int propertiesCount() const {return properties_.size_s();}
	
	//! Returns property with name "name"
	PIVariant property(const PIString & name) const {if (!properties_.contains(name)) return PIVariant(); return properties_.value(name);}
	PIVariant property(const char * name) const {return property(PIStringAscii(name));}
	
	//! Set property with name "name" to "value". If there is no such property in object it will be added
	void setProperty(const PIString & name, const PIVariant & value) {properties_[name] = value; propertyChanged(name);}
	void setProperty(const char * name, const PIVariant & value) {setProperty(PIStringAscii(name), value);}
	
	//! Returns if property with name "name" exists
	bool isPropertyExists(const PIString & name) const {return properties_.contains(name);}
	bool isPropertyExists(const char * name) const {return isPropertyExists(PIStringAscii(name));}
	
	void setThreadSafe(bool yes) {thread_safe_ = yes;}
	bool isThreadSafe() const {return thread_safe_;}
	
	void dump(const PIString & line_prefix = PIString()) const;
	
	
	PIStringList methodsEH();
	bool isMethodEHContains(const PIString & name) const;
	PIString methodEHArguments(const PIString & name) const;
	PIString methodEHFullFormat(const PIString & name) const;
	PIString methodEHFromAddr(const void * addr) const;
	
	/*
	template <typename RS, typename RD>
	static void piConnectU(PIObject * src, RS(*sig)(void*), PIObject * dest, RD(*slt)(void*), PIString signame) {
		src->connections << PIObject::Connection((void*)slt, (void*)sig, signame, dest);
	}
	template <typename RS, typename RD, typename A0>
	static void piConnectU(PIObject * src, RS(*sig)(void*,A0), PIObject * dest, RD(*slt)(void*,A0), PIString signame) {
		src->connections << PIObject::Connection((void*)slt, (void*)sig, signame, dest);
	}
	template <typename RS, typename RD, typename A0, typename A1>
	static void piConnectU(PIObject * src, RS(*sig)(void*,A0,A1), PIObject * dest, RD(*slt)(void*,A0,A1), PIString signame) {
		src->connections << PIObject::Connection((void*)slt, (void*)sig, signame, dest);
	}
	template <typename RS, typename RD, typename A0, typename A1, typename A2>
	static void piConnectU(PIObject * src, RS(*sig)(void*,A0,A1,A2), PIObject * dest, RD(*slt)(void*,A0,A1,A2), PIString signame) {
		src->connections << PIObject::Connection((void*)slt, (void*)sig, signame, dest);
	}
	template <typename RS, typename RD, typename A0, typename A1, typename A2, typename A3>
	static void piConnectU(PIObject * src, RS(*sig)(void*,A0,A1,A2,A3), PIObject * dest, RD(*slt)(void*,A0,A1,A2,A3), PIString signame) {
		src->connections << PIObject::Connection((void*)slt, (void*)sig, signame, dest);
	}
	template <typename RS, typename RD, typename A0, typename A1, typename A2, typename A3, typename A4>
	static void piConnectU(PIObject * src, RS(*sig)(void*,A0,A1,A2,A3,A4), PIObject * dest, RD(*slt)(void*,A0,A1,A2,A3,A4), PIString signame) {
		src->connections << PIObject::Connection((void*)slt, (void*)sig, signame, dest);
	}
	*/

	// / Direct connect
	static void piConnect(PIObject * src, const PIString & sig, PIObject * dest_o, void * dest, void * ev_h, void * e_h, int args, const char * loc);
	static bool piConnectU(PIObject * src, const PIString & ename, PIObject * dest_o, void * dest, const PIString & hname, const char * loc);
	
	// / Through names and mixed
	static void piConnect(const PIString & src, const PIString & sig, void * dest, void * ev_h);
	static void piConnect(PIObject * src, const PIString & sig, const PIString & dest, void * ev_h);
	static void piConnect(const PIString & src, const PIString & sig, const PIString & dest, void * ev_h);
	
	
	static void piDisconnect(PIObject * src, const PIString & sig, PIObject * dest, void * ev_h);
	static void piDisconnect(PIObject * src, const PIString & sig, PIObject * dest);
	
	//! Disconnect object "src" from all connections with event name "sig"
	static void piDisconnect(PIObject * src, const PIString & sig);
	
	//! Disconnect object "src" from all connections, i.e. all connections where object "src" is emitter
	static void piDisconnect(PIObject * src);
	
	// / Raise events
	static void raiseEvent(PIObject * sender, const PIString & event) {
		for (int j = 0; j < sender->connections.size_s(); ++j) {
			Connection & i(sender->connections[j]);
			if (i.event != event) continue;
			//piCout << uint(i.dest) << uint(i.dest_o);
			if (sender->thread_safe_) i.dest_o->mutex_.lock();
			i.dest_o->emitter_ = sender;
			((void( *)(void * ))i.slot)(i.dest);
			i.dest_o->emitter_ = 0;
			if (sender->thread_safe_) i.dest_o->mutex_.unlock();
		}
	}
	
	template <typename T0>
	static void raiseEvent(PIObject * sender, const PIString & event, const T0 & v0 = T0()) {
		for (int j = 0; j < sender->connections.size_s(); ++j) {
			Connection & i(sender->connections[j]);
			if (i.event != event) continue;
			if (sender->thread_safe_) i.dest_o->mutex_.lock();
			i.dest_o->emitter_ = sender;
			if (i.args_count == 0) ((void(*)(void *))i.slot)(i.dest);
			else ((void(*)(void * , T0))i.slot)(i.dest, v0);
			i.dest_o->emitter_ = 0;
			if (sender->thread_safe_) i.dest_o->mutex_.unlock();
		}
	}
	template <typename T0, typename T1>
	static void raiseEvent(PIObject * sender, const PIString & event, const T0 & v0 = T0(), const T1 & v1 = T1()) {
		for (int j = 0; j < sender->connections.size_s(); ++j) {
			Connection & i(sender->connections[j]);
			if (i.event != event) continue;
			if (sender->thread_safe_) i.dest_o->mutex_.lock();
			i.dest_o->emitter_ = sender;
			switch (i.args_count) {
			case 0: ((void(*)(void *))i.slot)(i.dest); break;
			case 1: ((void(*)(void * , T0))i.slot)(i.dest, v0); break;
			default: ((void(*)(void * , T0, T1))i.slot)(i.dest, v0, v1); break;
			}
			i.dest_o->emitter_ = 0;
			if (sender->thread_safe_) i.dest_o->mutex_.unlock();
		}
	}
	template <typename T0, typename T1, typename T2>
	static void raiseEvent(PIObject * sender, const PIString & event, const T0 & v0 = T0(), const T1 & v1 = T1(), const T2 & v2 = T2()) {
		for (int j = 0; j < sender->connections.size_s(); ++j) {
			Connection & i(sender->connections[j]);
			if (i.event != event) continue;
			if (sender->thread_safe_) i.dest_o->mutex_.lock();
			i.dest_o->emitter_ = sender;
			switch (i.args_count) {
			case 0: ((void(*)(void *))i.slot)(i.dest); break;
			case 1: ((void(*)(void * , T0))i.slot)(i.dest, v0); break;
			case 2: ((void(*)(void * , T0, T1))i.slot)(i.dest, v0, v1); break;
			default: ((void(*)(void * , T0, T1, T2))i.slot)(i.dest, v0, v1, v2); break;
			}
			i.dest_o->emitter_ = 0;
			if (sender->thread_safe_) i.dest_o->mutex_.unlock();
		}
	}
	template <typename T0, typename T1, typename T2, typename T3>
	static void raiseEvent(PIObject * sender, const PIString & event, const T0 & v0 = T0(), const T1 & v1 = T1(), const T2 & v2 = T2(), const T3 & v3 = T3()) {
		for (int j = 0; j < sender->connections.size_s(); ++j) {
			Connection & i(sender->connections[j]);
			if (i.event != event) continue;
			if (sender->thread_safe_) i.dest_o->mutex_.lock();
			i.dest_o->emitter_ = sender;
			switch (i.args_count) {
			case 0: ((void(*)(void *))i.slot)(i.dest); break;
			case 1: ((void(*)(void * , T0))i.slot)(i.dest, v0); break;
			case 2: ((void(*)(void * , T0, T1))i.slot)(i.dest, v0, v1); break;
			case 3: ((void(*)(void * , T0, T1, T2))i.slot)(i.dest, v0, v1, v2); break;
			default: ((void(*)(void * , T0, T1, T2, T3))i.slot)(i.dest, v0, v1, v2, v3); break;
			}
			i.dest_o->emitter_ = 0;
			if (sender->thread_safe_) i.dest_o->mutex_.unlock();
		}
	}

	/*
	// / Raise events through manager
	static void raiseEvent(const PIString & destObject, const PIString & name) {
		PIObject * dest = findByName(destObject);
		if (dest == 0) {
			cout << "PIObject::piConnect: can`t find PIObject with \"" << destObject << "\" name!" << endl;
			return;
		}
		raiseEvent(dest, name);
	}
	template <typename T0>
	static void raiseEvent(const PIString & destObject, const PIString & name, const T0 & v0 = T0()) {
		PIObject * dest = findByName(destObject);
		if (dest == 0) {
			cout << "PIObject::piConnect: can`t find PIObject with \"" << destObject << "\" name!" << endl;
			return;
		}
		raiseEvent<T0>(dest, name, v0);
	}
	template <typename T0, typename T1>
	static void raiseEvent(const PIString & destObject, const PIString & name, const T0 & v0 = T0(), const T1 & v1 = T1()) {
		PIObject * dest = findByName(destObject);
		if (dest == 0) {
			cout << "PIObject::piConnect: can`t find PIObject with \"" << destObject << "\" name!" << endl;
			return;
		}
		raiseEvent<T0, T1>(dest, name, v0, v1);
	}
	template <typename T0, typename T1, typename T2>
	static void raiseEvent(const PIString & destObject, const PIString & name, const T0 & v0 = T0(), const T1 & v1 = T1(), const T2 & v2 = T2()) {
		PIObject * dest = findByName(destObject);
		if (dest == 0) {
			cout << "PIObject::piConnect: can`t find PIObject with \"" << destObject << "\" name!" << endl;
			return;
		}
		raiseEvent<T0, T1, T2>(name, dest, v0, v1, v2);
	}
	template <typename T0, typename T1, typename T2, typename T3>
	static void raiseEvent(const PIString & destObject, const PIString & name, const T0 & v0 = T0(), const T1 & v1 = T1(), const T2 & v2 = T2(), const T3 & v3 = T3()) {
		PIObject * dest = findByName(destObject);
		if (dest == 0) {
			cout << "PIObject::piConnect: can`t find PIObject with \"" << destObject << "\" name!" << endl;
			return;
		}
		raiseEvent<T0, T1, T2, T3>(name,dest , v0, v1, v2, v3);
	}
	*/
	
	//! Returns PIObject* with name "name" or 0, if there is no object found
	static PIObject * findByName(const PIString & name) {
		piForeach (PIObject * i, PIObject::objects) {
			if (i->name() != name) continue;
			return i;
		}
		return 0;
	};
	
	bool isPIObject() const {return isPIObject(this);}
	bool execute(const PIString & method);
	static bool isPIObject(const PIObject * o) {return o->_signature_ == __PIOBJECT_SIGNATURE__;}
	static bool isPIObject(const void * o) {return isPIObject((PIObject*)o);}
	static bool execute(PIObject * o, const PIString & method) {return o->execute(method);}
	static bool execute(void * o, const PIString & method) {return ((PIObject*)o)->execute(method);}
	static PIString simplifyType(const char * a);

	struct __EHFunc {
		__EHFunc(): addr(0) {;}
		bool isNull() const {return addr == 0;}
		PIString arguments() const;
		PIString fullFormat() const;
		void * addr;
		PIString func_name;
		PIString type_ret;
		PIString scope;
		PIStringList types;
		PIStringList names;
	};
	struct __EHData {
		PISet<const void * > eh_set;
		PIMap<const void * , __EHFunc> eh_func;
	};
	typedef PIPair<const void * , __EHFunc> __EHPair;
	static PIMap<PIString, __EHData> __eh_data;
	static PIMutex __eh_mutex;
	
protected:
	
	//! Returns PIObject* which has raised an event. This value is correct only in definition of some event handler
	PIObject * emitter() const {return emitter_;}
	
	//! Virtual function executes after property with name "name" has been changed
	virtual void propertyChanged(const PIString & name) {}
	
	
	static const PIString __classNameS() {return PIStringAscii("PIObject");}
	
	EVENT(deleted)
	
//! \events
//! \{

	/** \fn void deleted()
	 * \brief Raise before object delete
	 * \note This event raised from destructor, so use only emitter() value,
	 * don`t try to cast deleted object to some subclass! */

//! \}

private:
	struct Connection {
		Connection(void * sl = 0, void * si = 0, const PIString & e = PIString(), PIObject * d_o = 0, void * d = 0, int ac = 0) {
			slot = sl;
			signal = si;
			event = e;
			dest_o = d_o;
			dest = d;
			args_count = ac;
		}
		void * slot;
		void * signal;
		PIString event;
		PIObject * dest_o;
		void * dest;
		int args_count;
	};
	
	PIVector<__EHFunc> findEH(const PIString & name) const;
	__EHFunc methodEH(const void * addr) const;
	void updateConnectors();
	
	PIVector<Connection> connections;
	typedef PIPair<PIString, PIVariant> Property;
	PIMap<PIString, PIVariant> properties_;
	
	static PIVector<PIObject * > objects;
	
	PISet<PIObject * > connectors;
	PIMutex mutex_, mutex_connect;
	PIObject * emitter_;
	bool thread_safe_;

};


void dumpApplication();
bool dumpApplicationToFile(const PIString & path);

#endif // PIOBJECT_H
