#include "dietfeatures.h"
#include <string.h>

#ifdef WANT_NON_COMPLIANT_STRNCAT
/* this implementation is not standards compliant.
 * the standard says that strncat(dest,"foobar",3) should write 'f', 'o'
 * and 'o'.  The programmer is then expected to overwrite the last byte
 * with '\0', which is often forgotten.  This implementation makes sure
 * the last written bytes is always '\0'. */
#endif

/* gcc is broken and has a non-SUSv2 compliant internal prototype.
 * This causes it to warn about a type mismatch here.  Ignore it. */
char *strncat(char *s, const char *t, size_t n) {
  char *dest=s;
  register char *max;
  s+=strlen(s);
#ifdef WANT_NON_COMPLIANT_STRNCAT
  if (__unlikely((max=s+n-1)<=s)) goto fini;
#else
  if (__unlikely((max=s+n)==s)) goto fini;
#endif
  for (;;) {
    if (__unlikely(!(*s = *t))) break; if (__unlikely(++s==max)) break; ++t;
#ifndef WANT_SMALL_STRING_ROUTINES
    if (__unlikely(!(*s = *t))) break; if (__unlikely(++s==max)) break; ++t;
    if (__unlikely(!(*s = *t))) break; if (__unlikely(++s==max)) break; ++t;
    if (__unlikely(!(*s = *t))) break; if (__unlikely(++s==max)) break; ++t;
#endif
  }
  *s=0;
fini:
  return dest;
}
