#include "dietfeatures.h"

char* strcpy(register char* s,register const char* t)
{
  char *dest=s;
  for (;;) {
    if (!(*s = *t)) return dest; ++s; ++t;
#ifndef WANT_SMALL_STRING_ROUTINES
    if (!(*s = *t)) return dest; ++s; ++t;
    if (!(*s = *t)) return dest; ++s; ++t;
    if (!(*s = *t)) return dest; ++s; ++t;
#endif
  }
}

