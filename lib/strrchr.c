#include <string.h>
#include "dietfeatures.h"

char *strrchr(const char *t, int c) {
  register char ch;
  register const char *l=0;

  ch = c;
  for (;;) {
    if (*t == ch) l=t; if (!*t) return (char*)l; ++t;
#ifndef WANT_SMALL_STRING_ROUTINES
    if (*t == ch) l=t; if (!*t) return (char*)l; ++t;
    if (*t == ch) l=t; if (!*t) return (char*)l; ++t;
    if (*t == ch) l=t; if (!*t) return (char*)l; ++t;
#endif
  }
  return (char*)l;
}
