#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int vfprintf(FILE *fstream, const char *format, va_list ap)
{
  char *tmp;
  size_t n=vsnprintf(0, 1000000, format, ap);
  tmp=alloca(n+2);
  if (tmp) {
    vsnprintf(tmp, n+1, format, ap);
    fwrite(tmp, n,1, fstream);
  }
  return n;
}

