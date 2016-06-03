#define _GNU_SOURCE
#include <string.h>
#include "dietfeatures.h"

char* stpncpy(char* dest, const char* src, size_t n) {
  char* x=memccpy(dest,src,0,n);
  if (x) {
#ifdef WANT_FULL_POSIX_COMPAT
    memset(x,0,n-(x-dest));
#endif
    return x-1;
  } else
    return dest+n;
}
