#ifndef _STDDEF_H
#define _STDDEF_H

/* ugh.  This is normally provided by gcc. */

typedef signed long ptrdiff_t;
typedef unsigned int size_t;
typedef int wchar_t;

#define NULL (void*)0

#define offsetof(type,member) ((size_t) &((type*)0)->member)

#endif
