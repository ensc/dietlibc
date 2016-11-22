#ifndef _DIET_STRING_H_
#define _DIET_STRING_H_

#include <endian.h>

/* STRALIGN returns the number of bytes to add to a pointer to make it
 * point to a (unsigned long)-aligned piece of memory */
#define STRALIGN(x) ((((uintptr_t)x+sizeof(long)-1)&(-sizeof(long)))-(uintptr_t)x)

/* MKW returns an unsigned long vector where every byte is x */
#define MKW(x) ((unsigned long)x*0x0101010101010101)

/*
#if __WORDSIZE == 64
# define MKW(x) (x|x<<8|x<<16|x<<24|x<<32|x<<40|x<<48|x<<56)
# define STRALIGN(x) (((unsigned long)x&7)?8-((unsigned long)x&7):0)
#else
# define MKW(x) (x|x<<8|x<<16|x<<24)
# define STRALIGN(x) (((unsigned long)x&3)?4-((unsigned long)x&3):0)
#endif
*/

/* GFC(x)    - returns first character */
/* INCSTR(x) - moves to next character */
#if __BYTE_ORDER == __LITTLE_ENDIAN
# define GFC(x) ((x)&0xff)
# define INCSTR(x) do { x >>= 8; } while (0)
#else
# define GFC(x) (((x)>>(sizeof(x)*8-8))&0xff)
# define INCSTR(x) do { x <<= 8; } while (0)
#endif

#define UNALIGNED(x,y) (((unsigned long)x & (sizeof (unsigned long)-1)) ^ ((unsigned long)y & (sizeof (unsigned long)-1)))

#endif /* _DIET_STRING_H_ */
