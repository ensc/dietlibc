#include <stdlib.h>

void* reallocarray(void* ptr, size_t nmemb, size_t size) {
  /* produce a compile-time error if uintmax_t is not larger than size_t */
  char dummy[(sizeof(uintmax_t)>=sizeof(size_t)*2)-1];
  (void)dummy;

  uintmax_t l=(uintmax_t)nmemb * size;
  if ((size_t)l != l) return 0;	// overflow
  return realloc(ptr,(size_t)l);
}
