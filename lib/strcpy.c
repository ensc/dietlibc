#include "dietfeatures.h"

char* strcpy(register char* s,register const char* t)
{
  char *dest=s;
#ifndef WANT_SMALL_STRING_ROUTINES
  for (;;) {
    if (!(*s = *t)) return dest; ++s; ++t;
    if (!(*s = *t)) return dest; ++s; ++t;
    if (!(*s = *t)) return dest; ++s; ++t;
    if (!(*s = *t)) return dest; ++s; ++t;
  }
#else
  while ((*dest++=*t++));
  return s;
#endif
}

