#include <sys/types.h>
#include <string.h>

int strncmp(const char *s1, const char *s2, size_t n) {
  return memccmp(s1,s2,0,n);
}
