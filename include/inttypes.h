#ifndef _INTTYPES_H
#define _INTTYPES_H

#include <sys/cdefs.h>

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef signed long int intptr_t;
typedef unsigned long int uintptr_t;

#ifndef __STRICT_ANSI__
__extension__ typedef signed long long int64_t;
__extension__ typedef unsigned long long uint64_t;
__extension__ typedef signed long long int intmax_t;
__extension__ typedef unsigned long long int uintmax_t;
#endif

#endif
