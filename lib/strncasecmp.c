#include <ctype.h>
#include <sys/types.h>
#include <string.h>

int strncasecmp(const char *s, const char *t, size_t n) {
  register char x;
  register const char* u=s+n;
  for (;;) {
    x = tolower(*s); if (x!=tolower(*t)) break; if (!x) break; if (++s>=u) return 0; ++t;
#ifndef WANT_SMALL_STRING_ROUTINES
    x = tolower(*s); if (x!=tolower(*t)) break; if (!x) break; if (++s>=u) return 0; ++t;
    x = tolower(*s); if (x!=tolower(*t)) break; if (!x) break; if (++s>=u) return 0; ++t;
    x = tolower(*s); if (x!=tolower(*t)) break; if (!x) break; if (++s>=u) return 0; ++t;
#endif
  }
  return ((int)(unsigned int)(unsigned char) x)
       - ((int)(unsigned int)(unsigned char) *t);
}
