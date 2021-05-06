
#ifndef PIP_EXPORT_H
#define PIP_EXPORT_H

#ifdef PIP_STATIC_DEFINE
#  define PIP_EXPORT
#  define PIP_NO_EXPORT
#else
#  ifndef PIP_EXPORT
#    ifdef pip_EXPORTS
        /* We are building this library */
#      define PIP_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define PIP_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef PIP_NO_EXPORT
#    define PIP_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef PIP_DEPRECATED
#  define PIP_DEPRECATED __attribute__ ((__deprecated__))
#  define PIP_DEPRECATED_EXPORT PIP_EXPORT __attribute__ ((__deprecated__))
#  define PIP_DEPRECATED_NO_EXPORT PIP_NO_EXPORT __attribute__ ((__deprecated__))
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define PIP_NO_DEPRECATED
#endif

#endif
