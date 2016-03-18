#define _GNU_SOURCE
#include <string.h>

char* stpncpy(char* dest, const char* src, size_t n) {
  char* x=memccpy(dest,src,0,n);
  if (x) {
    memset(x,0,n-(x-dest));
    return x-1;
  } else
    return dest+n;
}
