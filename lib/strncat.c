#include "dietfeatures.h"
#include <string.h>

char *strncat(char *s, const char *t, size_t n) {
  char *dest=s;
  register char *max=s+n-1;
  s+=strlen(s);
  for (;;) {
    if (!(*s = *t)) break; if (++s==max) break; ++t;
#ifndef WANT_SMALL_STRING_ROUTINES
    if (!(*s = *t)) break; if (++s==max) break; ++t;
    if (!(*s = *t)) break; if (++s==max) break; ++t;
    if (!(*s = *t)) break; if (++s==max) break; ++t;
#endif
  }
  *s=0;
  return dest;
}

