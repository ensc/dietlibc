#include <ctype.h>

int __isalnum_ascii(int c) {
  int d = c|0x20;
  return ((d >= 'a') & (d <= 'z')) | ((c >= '0') & (c <= '9'));
}

int isalnum(int c) __attribute__((weak,alias("__isalnum_ascii")));
