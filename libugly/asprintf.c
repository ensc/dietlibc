#include <stdarg.h>
#include <sys/types.h>
#include <stdlib.h>
#include "dietwarning.h"

extern int vsnprintf (char *str,size_t size,const char *format, va_list arg_ptr);

int asprintf(char **s, const char *format,...)
{
  int n;
  va_list arg_ptr;
  va_start(arg_ptr, format);
  n=vsnprintf(0,1000000,format,arg_ptr);
  va_end (arg_ptr);
  va_start (arg_ptr, format);
  if ((*s=malloc(n+2))) {
    n=vsnprintf(*s,n+1,format,arg_ptr);
    va_end(arg_ptr);
    return n;
  }
  return -1;
}
