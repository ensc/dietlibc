#define _POSIX_SOURCE
#define _XOPEN_SOURCE
#include <sys/types.h>
#include <string.h>

#undef strncpy
/* gcc is broken and has a non-SUSv2 compliant internal prototype.
 * This causes it to warn about a type mismatch here.  Ignore it. */
char *strncpy(char *dest, const char *src, size_t n);
char *strncpy(char *dest, const char *src, size_t n) {
  memccpy(dest,src,0,n);
  return dest;
}

char *strxfrm(char *dest, const char* src, size_t n)	__attribute__((weak,alias("strncpy")));
