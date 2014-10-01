#include <stdlib.h>
#include <endian.h>

#if __WORDSIZE == 64
typedef __uint128_t ulll;
#else
typedef uint64_t ulll;
#endif

void* reallocarray(void* ptr, size_t nmemb, size_t size) {
  /* produce a compile-time error if uintmax_t is not larger than size_t */
  typedef char compile_time_assertion[(sizeof(ulll)>=sizeof(size_t)*2)-1];

  ulll l=(ulll)nmemb * size;
  if ((size_t)l != l) return 0;	// overflow
  return realloc(ptr,(size_t)l);
}
