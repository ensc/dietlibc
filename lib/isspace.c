#include <ctype.h>

int __isspace_ascii(int ch)
{
  return ((ch==' ')||(ch=='\f')||(ch=='\t')||(ch=='\v')||(ch=='\r')||(ch=='\n'));
}

int isspace(int c) __attribute__((weak,alias("__isspace_ascii")));
