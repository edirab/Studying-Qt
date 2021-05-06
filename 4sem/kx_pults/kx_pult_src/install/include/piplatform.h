/*
    PIP - Platform Independent Primitives
    Platform and compiler macros
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

#ifndef PIPLATFORM_H
#define PIPLATFORM_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#  define WINDOWS
#  define ARCH_BITS_32
#endif
#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#  define WINDOWS
#  define ARCH_BITS_64
#endif
#if defined(__QNX__) || defined(__QNXNTO__)
#  define QNX
#  ifdef Q_OS_BLACKBERRY
#    define BLACKBERRY
#  endif
#endif
#ifdef __FreeBSD__
#  define FREE_BSD
#endif
#if defined(__APPLE__) || defined(__MACH__)
#  define MAC_OS
#endif
#if defined(__ANDROID__) || defined(_ANDROID_) || defined(ANDROID)
#  ifndef ANDROID
#    define ANDROID
#  endif
#endif
#ifndef WINDOWS
#  ifndef QNX
#    ifndef FREE_BSD
#      ifndef MAC_OS
#        ifndef ANDROID
#          ifndef BLACKBERRY
#            define LINUX
#          endif
#        endif
#      endif
#    endif
#  endif
#endif

#ifndef WINDOWS
#  if defined(__LP64__) || defined(_LP64_) || defined(LP64)
#    define ARCH_BITS_64
#  else
#    define ARCH_BITS_32
#  endif
#endif

#ifdef __GNUC__
#  define CC_GCC
#  define CC_GCC_VERSION  ((__GNUC__ << 8) | __GNUC_MINOR__)
#elif defined(_MSC_VER)
#  define CC_VC
#else
#  define CC_OTHER
#endif

#ifdef WINDOWS
#  ifdef CC_GCC
#    define typeof __typeof
#  endif
#else
#  define typeof __typeof__
#endif

#if defined(DOXYGEN) || defined(CC_GCC) || defined(PICODE)
#  undef PIP_EXPORT
#  define PIP_EXPORT
#  undef DEPRECATED
#  define DEPRECATED
#endif

#endif // PIPLATFORM_H
