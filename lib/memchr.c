#include <sys/types.h>

char *memchr(const char *s, int c, size_t n) {
  int i;
  for (i=n; i; --i)
    if (*s++==c)
      return (char*)s;
  return 0;
}
