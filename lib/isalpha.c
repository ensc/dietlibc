#include <ctype.h>

int __isalpha_ascii(int c) {
  return (c>='a' && c<='z') || (c>='A' && c<='Z');
}

int isalpha(int c) __attribute__((weak,alias("__isalpha_ascii")));
