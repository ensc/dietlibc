#include <stdarg.h>
#include <linux/types.h>
#include <stdlib.h>

int vsnprintf (char *str,size_t size,const char *format, va_list arg_ptr);

int sprintf(char *dest,const char *format,...)
{
  int n;
  va_list arg_ptr;
  va_start(arg_ptr, format);
  n=vsnprintf(dest,1000000,format,arg_ptr);
  va_end (arg_ptr);
  return n;
}
