#ifndef _SYS_CDEFS_H
#define _SYS_CDEFS_H

#ifndef __cplusplus
#define throw ()
#define __THROW
#else
#define __THROW throw ()
#endif

#ifndef __GNUC__
#define __attribute__(xyz)
#endif

#define __pure__
#ifdef __GNUC__
#if (__GNUC__ > 2) || ((__GNUC__ == 2) && (__GNUC_MINOR__ >= 96))
#undef __pure__
#define __pure__ __attribute__ ((__pure__))
#endif

#endif

#endif
