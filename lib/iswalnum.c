#include <ctype.h>
#include <wctype.h>

int __iswalnum__ascii(wint_t c);
int __iswalnum__ascii(wint_t c) {
  return (((unsigned char)c == c)?isalnum(c):0);
}

int iswxalnum(wint_t c) __attribute__((weak,alias("__iswxalnum_ascii")));
