#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int vfprintf(FILE *fstream, const char *format, va_list ap)
{
  char *tmp;
  va_list cp_ap;
  size_t n = 0;

  va_copy(cp_ap, ap);
  n=vsnprintf(0, 1000000, format, cp_ap);
  tmp=alloca(n+2);
  vsnprintf(tmp, n+1, format, ap);
  fwrite(tmp, n,1, fstream);
  return n;
}

