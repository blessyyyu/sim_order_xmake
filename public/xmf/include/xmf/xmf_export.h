
#ifndef XMF_EXPORT_H
#define XMF_EXPORT_H

#ifdef XMF_STATIC_DEFINE
#  define XMF_EXPORT
#  define XMF_NO_EXPORT
#else
#  ifndef XMF_EXPORT
#    ifdef xmf_EXPORTS
        /* We are building this library */
#      define XMF_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define XMF_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef XMF_NO_EXPORT
#    define XMF_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef XMF_DEPRECATED
#  define XMF_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef XMF_DEPRECATED_EXPORT
#  define XMF_DEPRECATED_EXPORT XMF_EXPORT XMF_DEPRECATED
#endif

#ifndef XMF_DEPRECATED_NO_EXPORT
#  define XMF_DEPRECATED_NO_EXPORT XMF_NO_EXPORT XMF_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef XMF_NO_DEPRECATED
#    define XMF_NO_DEPRECATED
#  endif
#endif

#endif /* XMF_EXPORT_H */
