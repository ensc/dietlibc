#include <ctype.h>

int __ispunct_ascii(int c) {
  return isprint(c) && !( isalnum(c) || isspace(c) );
}

int ispunct(int c) __attribute__((weak,alias("__ispunct_ascii")));
