#include <ctype.h>
#include "dietfeatures.h"

int strcasecmp(register const char *s,register const char *t) {
  register char x;

  for (;;) {
    x = tolower(*s); if (x != tolower(*t)) break; if (!x) break; ++s; ++t;
#ifndef WANT_SMALL_STRING_ROUTINES
    x = tolower(*s); if (x != tolower(*t)) break; if (!x) break; ++s; ++t;
    x = tolower(*s); if (x != tolower(*t)) break; if (!x) break; ++s; ++t;
    x = tolower(*s); if (x != tolower(*t)) break; if (!x) break; ++s; ++t;
#endif
  }
  return ((int)(unsigned int)(unsigned char) x)
       - ((int)(unsigned int)(unsigned char) *t);
}
