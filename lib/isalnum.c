#include <ctype.h>

int __isalnum_ascii(int c) {
  return isalpha(c) || isdigit(c);
}

int isalnum(int c) __attribute__((weak,alias("__isalnum_ascii")));
