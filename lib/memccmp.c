#define _POSIX_SOURCE
#define _XOPEN_SOURCE
#include <sys/types.h>
#include <string.h>

int memccmp(const void *dst, const void *src, int c, size_t count)
{
  register const char *a = dst;
  register const char *b = src;
  while (count--)
  {
    register int res=(*a - *b);
    if (res) return res;
    if (*a==c) return 0;
    ++a; ++b;
  }
  return 0;
}
