#ifndef _STDDEF_H
#define _STDDEF_H

/* ugh.  This is normally provided by gcc. */

typedef signed long ptrdiff_t;
typedef unsigned long size_t;
typedef int wchar_t;

#ifndef NULL
#define NULL (void*)0
#endif

#define offsetof(type,member) ((size_t) &((type*)0)->member)

#endif
