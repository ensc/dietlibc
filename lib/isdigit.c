#include <ctype.h>

int __isdigit_ascii(int c) {
  return (c>='0' && c<='9');
}

int isdigit(int c) __attribute__((weak,alias("__isdigit_ascii")));
