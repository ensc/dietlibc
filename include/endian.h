#ifndef	_ENDIAN_H
#define	_ENDIAN_H

#define	__LITTLE_ENDIAN	1234
#define	__BIG_ENDIAN	4321

#ifdef __i386__
#define __BYTE_ORDER __LITTLE_ENDIAN
#define __FLOAT_WORD_ORDER __BYTE_ORDER
#endif

#ifdef __sparc__
#define __BYTE_ORDER __BIG_ENDIAN
#define __FLOAT_WORD_ORDER __BYTE_ORDER
#endif

#ifdef __mips__
#ifdef __MIPSEB__
#define __BIG_ENDIAN__
#endif
#endif

#ifndef __BYTE_ORDER
#ifdef __BIG_ENDIAN__
#define __BYTE_ORDER __BIG_ENDIAN
#define __FLOAT_WORD_ORDER __BYTE_ORDER
#else
#define __BYTE_ORDER __LITTLE_ENDIAN
#define __FLOAT_WORD_ORDER __BYTE_ORDER
#endif
#endif

#define LITTLE_ENDIAN	__LITTLE_ENDIAN
#define BIG_ENDIAN	__BIG_ENDIAN
#define BYTE_ORDER	__BYTE_ORDER

#if __BYTE_ORDER == __LITTLE_ENDIAN
# define __LONG_LONG_PAIR(HI, LO) LO, HI
#elif __BYTE_ORDER == __BIG_ENDIAN
# define __LONG_LONG_PAIR(HI, LO) HI, LO
#endif

#endif
