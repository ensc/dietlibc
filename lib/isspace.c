#include <ctype.h>

int __isspace_ascii(int ch)
{
  return ((ch > 8) & (ch < 14)) | (ch == ' ');
}

int isspace(int c) __attribute__((weak,alias("__isspace_ascii")));
