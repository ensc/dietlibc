#include <endian.h>
#include "dietfeatures.h"
#include <string.h>

#ifdef WANT_SMALL_STRING_ROUTINES
size_t strlen(const char *s) {
  register int i;
  if (!s) return 0;
  for (i=0; *s; ++s) ++i;
  return i;
}
#else
static const unsigned long magic = 0x01010101;

size_t strlen(const char *s)
{
  const char *t = s;
  unsigned long word;

  if (!s) return 0;

  /* Byte compare up until word boundary */
  for (; ((unsigned long) t & 3); t++)
    if (!*t) return t - s;

  /* Word compare */
  do {
    word = *((unsigned long *) t); t += 4;
    word = (word - magic) &~ word;
    word &= (magic << 7);
  } while (word == 0);

#if BYTE_ORDER == LITTLE_ENDIAN
  /* word & 0x80808080 == word */
  word = (word - 1) & (magic << 10);
  word += (word << 8) + (word << 16);
  t += word >> 26;
#else
  if ((word & 0x80800000) == 0) {
    word <<= 16;
    t += 2;
  }
  if ((word & 0x80000000) == 0) t += 1;
#endif
  return ((const char *) t) - 4 - s;
}
#endif
