#include <ctype.h>

int __islower_ascii(int c) {
  return (c>='a' && c<='z');
}

int islower(int c) __attribute__((weak,alias("__islower_ascii")));
