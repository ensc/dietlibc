#ifndef _STDARG_H
#define _STDARG_H

#include <endian.h>

typedef char* va_list;

#define va_end(ap) ap=0

#if __WORDSIZE == 64
#define va_start(ap,argn) ap=((char*)&argn)+8
#else
#define va_start(ap,argn) ap=((char*)&argn)+4
#endif

#define va_arg(ap,type) (ap+=sizeof(type), *(type*)((void*)ap-sizeof(type)))

#endif
