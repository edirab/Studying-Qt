/*! \file pibase.h
 * \brief Base types and functions
 *
 * This file implements first layer above the system and
 * declares some basic useful functions
*/
/*
	PIP - Platform Independent Primitives
	Base types and functions
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

#ifndef PIBASE_H
#define PIBASE_H

#include "piversion.h"
#include "piplatform.h"
#include "pip_export.h"

//! Version of PIP in hex - 0x##(Major)##(Minor)##(Revision)
#define PIP_VERSION ((PIP_VERSION_MAJOR << 16) | (PIP_VERSION_MINOR < 8) | PIP_VERSION_REVISION)

#ifdef DOXYGEN

//! Major value of PIP version
#  define PIP_VERSION_MAJOR

//! Minor value of PIP version
#  define PIP_VERSION_MINOR

//! Revision value of PIP version
#  define PIP_VERSION_REVISION

//! Suffix of PIP version
#  define PIP_VERSION_SUFFIX

//! Macro is defined when compile-time debug is enabled
#  define PIP_DEBUG

//! Macro is defined when host is any Windows
#  define WINDOWS

//! Macro is defined when host is QNX or Blackberry
#  define QNX

//! Macro is defined when host is Blackberry
#  define BLACKBERRY

//! Macro is defined when host is FreeBSD
#  define FREE_BSD

//! Macro is defined when host is Mac OS
#  define MAC_OS

//! Macro is defined when host is Android
#  define ANDROID

//! Macro is defined when host is any Linux
#  define LINUX

//! Macro is defined when compiler is GCC or MinGW
#  define CC_GCC

//! Macro is defined when PIP is decided that host is support language
#  define HAS_LOCALE

//! Macro is defined when compiler is Visual Studio
#  define CC_VC

//! Macro is defined when compiler is unknown
#  define CC_OTHER

//! Macro is defined when PIP can use "rt" library for "PITimer::ThreadRT" timers implementation
#  define PIP_TIMER_RT

//! Define this macro to use STL implementation of containers, else PIP implementation will be used
#  define PIP_CONTAINERS_STL

#endif

#include <stdarg.h>
#include <stddef.h>
#ifdef WINDOWS
#  include <windef.h>
#  include <winbase.h>
#  include <stdlib.h>
#  ifdef CC_VC
#    define SHUT_RDWR 2
#    pragma comment(lib, "Ws2_32.lib")
#    pragma comment(lib, "Iphlpapi.lib")
#    pragma comment(lib, "Psapi.lib")
#  else
#    define SHUT_RDWR SD_BOTH
#  endif
   typedef int socklen_t;
   typedef void(*PINtSetTimerResolution)(ULONG, BOOLEAN, PULONG);
   extern long long __pi_perf_freq;
   extern PINtSetTimerResolution setTimerResolutionAddr;
   inline int random() {return rand();}
#endif

#ifdef ANDROID
#  define tcdrain(fd) ioctl(fd, TCSBRK, 1)
   //inline int wctomb(char * c, wchar_t w) {*c = ((char * )&w)[0]; return 1;}
   //inline int mbtowc(wchar_t * w, const char * c, size_t) {*w = ((wchar_t * )&c)[0]; return 1;}
#endif

#ifdef MAC_OS
#  define environ (*_NSGetEnviron())
   typedef long time_t;
#endif

#ifdef LINUX
#  define environ __environ
#endif

#ifdef FREE_BSD
   extern char ** environ;
#endif


#ifdef CC_GCC
#  undef DEPRECATED
#  define DEPRECATED __attribute__((deprecated))
#  if CC_GCC_VERSION > 0x025F // > 2.95
#    ifdef LINUX
#      define HAS_LOCALE
#    endif
#    ifdef MAC_OS
#      pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#    else
#      pragma GCC diagnostic ignored "-Wpragmas"
#      pragma GCC diagnostic ignored "-Waggressive-loop-optimizations"
#    endif
#    pragma GCC diagnostic ignored "-Wformat"
#    pragma GCC diagnostic ignored "-Wformat-extra-args"
#    pragma GCC diagnostic ignored "-Wstrict-aliasing"
#    pragma GCC diagnostic ignored "-Wsign-compare"
#  endif
#  ifdef ANDROID
#    pragma GCC diagnostic ignored "-Wunused-parameter"
#    pragma GCC diagnostic ignored "-Wextra"
#    pragma GCC diagnostic ignored "-Wliteral-suffix"
#  endif
#endif

#ifdef CC_VC
#  undef DEPRECATED
#  define DEPRECATED
#  pragma warning(disable: 4018)
#  pragma warning(disable: 4061)
#  pragma warning(disable: 4100)
#  pragma warning(disable: 4239)
#  pragma warning(disable: 4242)
#  pragma warning(disable: 4244)
#  pragma warning(disable: 4251)
#  pragma warning(disable: 4365)
#  pragma warning(disable: 4512)
#  pragma warning(disable: 4668)
#  pragma warning(disable: 4710)
#  pragma warning(disable: 4800)
#  pragma warning(disable: 4820)
#  pragma warning(disable: 4986)
#  pragma warning(disable: 4996)
#  ifdef ARCH_BITS_32
	 typedef long ssize_t;
#  else
	 typedef long long ssize_t;
#  endif
#endif

#ifdef CC_OTHER
#  undef DEPRECATED
#  define DEPRECATED
#endif


// Private data macros

#define PRIVATE_DECLARATION \
	struct __Private__; \
	friend struct __Private__; \
	struct __PrivateInitializer__ { \
		__PrivateInitializer__(); \
		__PrivateInitializer__(const __PrivateInitializer__ & o); \
		~__PrivateInitializer__(); \
		__PrivateInitializer__ & operator =(const __PrivateInitializer__ & o); \
		__Private__ * p; \
	}; \
	__PrivateInitializer__ __privateinitializer__;

#define PRIVATE_DEFINITION_START(c) \
	struct c::__Private__ {

#define PRIVATE_DEFINITION_END(c) \
	}; \
	c::__PrivateInitializer__::__PrivateInitializer__() {p = new c::__Private__();} \
	c::__PrivateInitializer__::__PrivateInitializer__(const c::__PrivateInitializer__ & o) {if (p) delete p; p = new c::__Private__();} \
	c::__PrivateInitializer__::~__PrivateInitializer__() {delete p; p = 0;} \
	c::__PrivateInitializer__ & c::__PrivateInitializer__::operator =(const c::__PrivateInitializer__ & o) {if (p) delete p; p = new c::__Private__(); return *this;}

#define PRIVATE (__privateinitializer__.p)


//! Macro used for infinite loop
#define FOREVER for (;;)

//! Macro used for infinite wait
#define FOREVER_WAIT FOREVER msleep(1);

//! Macro used for infinite wait
#define WAIT_FOREVER FOREVER msleep(1);


//! global variable enabling output to piCout, default is true
extern PIP_EXPORT bool piDebug;

//! global variable that set minimum real update interval
//! for function PIInit::mountInfo(), default is 10000 ms
extern PIP_EXPORT double piMountInfoRefreshIntervalMs;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;
typedef long long llong;
typedef long double ldouble;

/*! \brief Templated function for swap two values
 * \details Example:\n \snippet piincludes.cpp swap */
template<typename T> inline void piSwap(T & f, T & s) {T t = f; f = s; s = t;}

/*! \brief Templated function for swap two values without "="
 * \details Example:\n \snippet piincludes.cpp swapBinary */
template<typename T> inline void piSwapBinary(T & f, T & s) {
	static size_t j = (sizeof(T) / sizeof(size_t)), bs = j * sizeof(size_t), bf = sizeof(T);
	size_t i = 0;
	for (i = 0; i < j; ++i) {
		((size_t*)(&f))[i] ^= ((size_t*)(&s))[i];
		((size_t*)(&s))[i] ^= ((size_t*)(&f))[i];
		((size_t*)(&f))[i] ^= ((size_t*)(&s))[i];
	}
	for (i = bs; i < bf; ++i) {
		((uchar*)(&f))[i] ^= ((uchar*)(&s))[i];
		((uchar*)(&s))[i] ^= ((uchar*)(&f))[i];
		((uchar*)(&f))[i] ^= ((uchar*)(&s))[i];
	}
}

/*! \brief Templated function return round of float falue
 * \details Round is the nearest integer value \n
 * There are some macros:
 * - \c piRoundf for "float"
 * - \c piRoundd for "double"
 *
 * Example:
 * \snippet piincludes.cpp round */
template<typename T> inline int piRound(const T & v) {return int(v >= T(0.) ? v + T(0.5) : v - T(0.5));}

/*! \brief Templated function return floor of float falue
 * \details Floor is the largest integer that is not greater than value \n
 * There are some macros:
 * - \c piFloorf for "float"
 * - \c piFloord for "double"
 *
 * Example:
 * \snippet piincludes.cpp floor */
template<typename T> inline int piFloor(const T & v) {return v < T(0) ? int(v) - 1 : int(v);}

/*! \brief Templated function return ceil of float falue
 * \details Ceil is the smallest integer that is not less than value \n
 * There are some macros:
 * - \c piCeilf for "float"
 * - \c piCeild for "double"
 *
 * Example:
 * \snippet piincludes.cpp ceil */
template<typename T> inline int piCeil(const T & v) {return v < T(0) ? int(v) : int(v) + 1;}

/*! \brief Templated function return absolute of numeric falue
 * \details Absolute is the positive or equal 0 value \n
 * There are some macros:
 * - \c piAbss for "short"
 * - \c piAbsi for "int"
 * - \c piAbsl for "long"
 * - \c piAbsll for "llong"
 * - \c piAbsf for "float"
 * - \c piAbsd for "double"
 *
 * Example:
 * \snippet piincludes.cpp abs */
template<typename T> inline T piAbs(const T & v) {return (v >= T(0) ? v : -v);}

/*! \brief Templated function return minimum of two values
 * \details There are some macros:
 * - \c piMins for "short"
 * - \c piMini for "int"
 * - \c piMinl for "long"
 * - \c piMinll for "llong"
 * - \c piMinf for "float"
 * - \c piMind for "double"
 *
 * Example:
 * \snippet piincludes.cpp min2 */
template<typename T> inline T piMin(const T & f, const T & s) {return ((f > s) ? s : f);}

/*! \brief Templated function return minimum of tree values
 * \details There are some macros:
 * - \c piMins for "short"
 * - \c piMini for "int"
 * - \c piMinl for "long"
 * - \c piMinll for "llong"
 * - \c piMinf for "float"
 * - \c piMind for "double"
 *
 * Example:
 * \snippet piincludes.cpp min3 */
template<typename T> inline T piMin(const T & f, const T & s, const T & t) {return ((f < s && f < t) ? f : ((s < t) ? s : t));}

/*! \brief Templated function return maximum of two values
 * \details There are some macros:
 * - \c piMaxs for "short"
 * - \c piMaxi for "int"
 * - \c piMaxl for "long"
 * - \c piMaxll for "llong"
 * - \c piMaxf for "float"
 * - \c piMaxd for "double"
 *
 * Example:
 * \snippet piincludes.cpp max2 */
template<typename T> inline T piMax(const T & f, const T & s) {return ((f < s) ? s : f);}

/*! \brief Templated function return maximum of tree values
 * \details There are some macros:
 * - \c piMaxs for "short"
 * - \c piMaxi for "int"
 * - \c piMaxl for "long"
 * - \c piMaxll for "llong"
 * - \c piMaxf for "float"
 * - \c piMaxd for "double"
 *
 * Example:
 * \snippet piincludes.cpp max3 */
template<typename T> inline T piMax(const T & f, const T & s, const T & t) {return ((f > s && f > t) ? f : ((s > t) ? s : t));}

/*! \brief Templated function return clamped value
 * \details Clamped is the not greater than "max" and not lesser than "min" value \n
 * There are some macros:
 * - \c piClamps for "short"
 * - \c piClampi for "int"
 * - \c piClampl for "long"
 * - \c piClampll for "llong"
 * - \c piClampf for "float"
 * - \c piClampd for "double"
 *
 * Example:
 * \snippet piincludes.cpp clamp */
template<typename T> inline T piClamp(const T & v, const T & min, const T & max) {return (v > max ? max : (v < min ? min : v));}

/// Function inverse byte order in memory block
inline void piLetobe(void * data, int size) {
	for (int i = 0; i < size / 2; i++)
		piSwap<uchar>(((uchar*)data)[size - i - 1], ((uchar*)data)[i]);
}

/// \brief Templated function that inverse byte order of value "v"
template<typename T> inline void piLetobe(T * v) {piLetobe(v, sizeof(T));}

/*! \brief Templated function that returns "v" with inversed byte order
 * \details This function used to convert values between little and big endian \n
 * There are some macros:
 * - \c piLetobes for "ushort"
 * - \c piLetobei for "uint"
 * - \c piLetobel for "ulong"
 * - \c piLetobell for "ullong"
 *
 * Example:
 * \snippet piincludes.cpp letobe */
template<typename T> inline T piLetobe(const T & v) {T tv(v); piLetobe(&tv, sizeof(T)); return tv;}

// specialization
template<> inline ushort piLetobe(const ushort & v) {return (v << 8) | (v >> 8);}
template<> inline uint piLetobe(const uint & v) {return (v >> 24) | ((v >> 8) & 0xFF00) | ((v << 8) & 0xFF0000) | ((v << 24) & 0xFF000000);}
template<> inline float piLetobe(const float & f) {
	uint v = *((uint *)&f);
	v = (v >> 24) | ((v >> 8) & 0xFF00) | ((v << 8) & 0xFF0000) | ((v << 24) & 0xFF000000);
	return *((float *)&v);
}

DEPRECATED inline ushort letobe_s(const ushort & v) {return (v << 8) | (v >> 8);}
DEPRECATED inline uint letobe_i(const uint & v) {return (v >> 24) | ((v >> 8) & 0xFF00) | ((v << 8) & 0xFF0000) | ((v << 24) & 0xFF000000);}

#ifdef DOXYGEN

/// \deprecated \brief Use \a piLetobe() instead of this function
ushort letobe_s(ushort v) {return (v << 8) | (v >> 8);}

/// \deprecated \brief Use \a piLetobe() instead of this function
uint letobe_i(uint v) {return (v >> 24) | ((v >> 8) & 0xFF00) | ((v << 8) & 0xFF0000) | ((v << 24) & 0xFF000000);}

#endif

#define piRoundf piRound<float>
#define piRoundd piRound<double>
#define piFloorf piFloor<float>
#define piFloord piFloor<double>
#define piCeilf piCeil<float>
#define piCeild piCeil<double>
#define piAbss piAbs<short>
#define piAbsi piAbs<int>
#define piAbsl piAbs<long>
#define piAbsll piAbs<llong>
#define piAbsf piAbs<float>
#define piAbsd piAbs<double>
#define piMins piMin<short>
#define piMini piMin<int>
#define piMinl piMin<long>
#define piMinll piMin<llong>
#define piMinf piMin<float>
#define piMind piMin<double>
#define piMaxs piMax<short>
#define piMaxi piMax<int>
#define piMaxl piMax<long>
#define piMaxll piMax<llong>
#define piMaxf piMax<float>
#define piMaxd piMax<double>
#define piClamps piClamp<short>
#define piClampi piClamp<int>
#define piClampl piClamp<long>
#define piClampll piClamp<llong>
#define piClampf piClamp<float>
#define piClampd piClamp<double>
#define piLetobes piLetobe<ushort>
#define piLetobei piLetobe<uint>
#define piLetobel piLetobe<ulong>
#define piLetobell piLetobe<ullong>
#define piLetobef piLetobe<float>


#endif // PIBASE_H
