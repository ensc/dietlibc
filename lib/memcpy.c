#include <sys/types.h>

/* gcc is broken and has a non-SUSv2 compliant internal prototype.
 * This causes it to warn about a type mismatch here.  Ignore it. */
void* memcpy(void* dst, const void* src, size_t count) {
  register char *d=dst;
  register const char *s=src;
  ++count;	/* this actually produces better code than using count-- */
  while (--count) {
    *d = *s;
    ++d; ++s;
  }
  return dst;
}
