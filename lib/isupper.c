#include <ctype.h>

int __isupper_ascii(int c) {
  return (c>='A' && c<='Z');
}

int isupper(int c) __attribute__((weak,alias("__isupper_ascii")));
