#ifndef _SYS_CDEFS_H
#define _SYS_CDEFS_H

#ifndef __cplusplus
#define __THROW
#define __BEGIN_DECLS
#define __END_DECLS
#else
#define __THROW throw ()
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#endif

#ifndef __GNUC__
#define __attribute__(xyz)
#define __extension__
#endif

#define __pure__
#ifdef __GNUC__
#if (__GNUC__ > 2) || ((__GNUC__ == 2) && (__GNUC_MINOR__ >= 96))
#undef __pure__
#define __pure__ __attribute__ ((__pure__))
#endif

#if (__GNUC__ == 2) && (__GNUC_MINOR__ < 95)
#define __restrict__
#endif

#ifndef __STRICT_ANSI__
#if (__GNUC__ < 3)
#define __builtin_expect(foo,bar) (foo)
#define expect(foo,bar) (foo)
#define __malloc__
#else
#define expect(foo,bar) __builtin_expect(foo,bar)
#define __malloc__ __attribute__((malloc))
#endif
#endif
#endif

#define __P(x) x

#define __ptr_t void*

#ifdef __STRICT_ANSI__
#define inline
#endif

#ifndef __GNUC__
#define __extension__
#endif

#ifndef __i386__
#define regparm(x)
#endif

#endif
