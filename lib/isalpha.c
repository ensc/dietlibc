#include <ctype.h>

int __isalpha_ascii(int c) {
  int d=c|0x20;
  return (d-'a'<='z'-'a');
}

int isalpha(int c) __attribute__((weak,alias("__isalpha_ascii")));
