#ifndef _STDDEF_H
#define _STDDEF_H

/* ugh.  This is normally provided by gcc. */

#ifdef __GNUC__
typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __SIZE_TYPE__ size_t;
typedef __WCHAR_TYPE__ wchar_t;
#else
typedef signed long ptrdiff_t;
typedef unsigned long size_t;
typedef int wchar_t;
#endif

#ifndef NULL
#define NULL (void*)0
#endif

#undef offsetof
#define offsetof(type,member) ((size_t) &((type*)0)->member)

#endif
