#include "dietfeatures.h"
#include <string.h>

/* this implementation is not standards compliant.
 * the standard says that strncat(dest,"foobar",3) should write 'f', 'o'
 * and 'o'.  The programmer is then expected to overwrite the last byte
 * with '\0', which is often forgotten.  This implementation makes sure
 * the last written bytes is always '\0'. */

char *strncat(char *s, const char *t, size_t n) {
  char *dest=s;
  register char *max;
  s+=strlen(s);
  max=s+n-1;
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

