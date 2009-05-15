#include <string.h>

char *mempcpy(char *dst, const char *src, size_t n) {
  size_t i;
  for (i=0; i<n; ++i)
    dst[i]=src[i];
  return dst+i;
}
