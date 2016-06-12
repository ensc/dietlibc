#define _POSIX_SOURCE
#define _XOPEN_SOURCE
#include <sys/types.h>
#include <string.h>
#include <emmintrin.h>

void *memccpy(void *dst, const void *src, int c, size_t count)
{
  char *a = dst;
  const char *b = src;
  size_t i;
  if (count>=16) {
    __m128i cmpval=_mm_set1_epi8((unsigned char)c);
    unsigned int mod=(count%16);
    count-=mod;
    for (i=0; i<count; ) {
      __m128i tmp=_mm_loadu_si128((__m128i*)(b+i));
      int cmp=_mm_movemask_epi8(_mm_cmpeq_epi8(cmpval,tmp));
      if (cmp) {
	int pos=__builtin_ctz(cmp);
	/* Now we need to write a partial vector.
	 * If we write the bytes one by one, we'll eat 3-4 cycles cache
	 * access penalty per byte we read, so we'd rather shift the
	 * offset backwards and read+write one vector. See if we copied
	 * enough data already for that not to be a buffer underflow */
	if (i+pos>=15) {
	  /* yes! */
	  _mm_storeu_si128((__m128i*)(a+i+pos-15),_mm_loadu_si128((__m128i*)(b+i+pos-15)));
	  return a+i+pos+1;
	}
	/* nope :-( */
	count=i+pos+1;
	for (; i<count; ++i)
	  a[i]=b[i];
	return a+i;
      }
      _mm_storeu_si128((__m128i*)(a+i),tmp);
      i+=mod; mod=16;
    }
    return 0;
  }
  for (i=0; i<count; ++i)
    if ((a[i]=b[i])==c)
      return a+i+1;
  return 0;
}
