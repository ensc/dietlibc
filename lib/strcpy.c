#include "dietfeatures.h"
#include <string.h>

char* strcpy(register char* s,register const char* t)
{
  char *dest=s;
#ifndef WANT_SMALL_STRING_ROUTINES
  for (;;) {
    if (__unlikely(!(*s = *t))) return dest; ++s; ++t;
    if (__unlikely(!(*s = *t))) return dest; ++s; ++t;
    if (__unlikely(!(*s = *t))) return dest; ++s; ++t;
    if (__unlikely(!(*s = *t))) return dest; ++s; ++t;
  }
#else
  while (__likely(*dest++=*t++));
  return s;
#endif
}

