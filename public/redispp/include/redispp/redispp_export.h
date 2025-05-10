
#ifndef REDISPP_EXPORT_H
#define REDISPP_EXPORT_H

#ifdef REDISPP_STATIC_DEFINE
#  define REDISPP_EXPORT
#  define REDISPP_NO_EXPORT
#else
#  ifndef REDISPP_EXPORT
#    ifdef redispp_EXPORTS
        /* We are building this library */
#      define REDISPP_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define REDISPP_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef REDISPP_NO_EXPORT
#    define REDISPP_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef REDISPP_DEPRECATED
#  define REDISPP_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef REDISPP_DEPRECATED_EXPORT
#  define REDISPP_DEPRECATED_EXPORT REDISPP_EXPORT REDISPP_DEPRECATED
#endif

#ifndef REDISPP_DEPRECATED_NO_EXPORT
#  define REDISPP_DEPRECATED_NO_EXPORT REDISPP_NO_EXPORT REDISPP_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef REDISPP_NO_DEPRECATED
#    define REDISPP_NO_DEPRECATED
#  endif
#endif

#endif /* REDISPP_EXPORT_H */
