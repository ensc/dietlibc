#include <ctype.h>

int __iscntrl_ascii(int c) {
  return (c<32) || (c==127);
}

int iscntrl(int c) __attribute__((weak,alias("__iscntrl_ascii")));
