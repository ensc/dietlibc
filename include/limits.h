#ifndef _LIMITS_H
#define _LIMITS_H

#include <linux/limits.h>

#define __WORDSIZE 32

#ifdef __alpha__
#undef __WORDSIZE
#define __WORDSIZE 64
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define CHAR_BIT 8

#define SCHAR_MIN (-128)
#define SCHAR_MAX 0x7f
#define UCHAR_MAX 0xff

#define SHRT_MIN (-SHRT_MAX-1)
#define SHRT_MAX 0x7fff
#define USHRT_MAX 0xffff

#define INT_MIN (-INT_MAX-1)
#define INT_MAX 0x7fffffff
#define UINT_MAX 0xffffffff

#if __WORDSIZE == 64
#define LONG_MAX 9223372036854775807L
#define ULONG_MAX 18446744073709551615UL
#else
#define LONG_MAX 2147483647L
#define ULONG_MAX 4294967295UL
#endif
#define LONG_MIN (-LONG_MAX - 1L)

#define LLONG_MAX 9223372036854775807LL
#define LLONG_MIN (-LLONG_MAX - 1LL)

/* Maximum value an `unsigned long long int' can hold.  (Minimum is 0.)  */
#define ULLONG_MAX 18446744073709551615ULL

#endif
