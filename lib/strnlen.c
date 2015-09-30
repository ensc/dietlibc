#include <string.h>

size_t strnlen(const char *s,size_t maxlen) {
  const char* x=memchr(s,0,maxlen);
  if (!x) x=s+maxlen;
  return x-s;
}

