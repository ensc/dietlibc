#ifndef _STDARG_H
#define _STDARG_H

#include <endian.h>

#ifdef __GNUC__
#if (__GNUC__ > 2) || ((__GNUC__ == 2) && (__GNUC_MINOR__ > 96)) || ((__GNUC__ == 2) && (__GNUC_MINOR__ == 96) && defined(__alpha__))


typedef __builtin_va_list va_list;
#define va_start(v,l)	__builtin_stdarg_start((v),l)
#define va_end		__builtin_va_end
#define va_arg		__builtin_va_arg
#define __va_copy(d,s)	__builtin_va_copy((d),(s))

#endif
#endif

#ifndef va_end
#include <stdarg-cruft.h>
#endif

#if !defined(__STRICT_ANSI__) || __STDC_VERSION__ + 0 >= 199900L
#define va_copy(d,s)	__va_copy(d,s)
#endif

#endif
