#include <stdarg.h>

int vsscanf(const char *str, const char *format, va_list arg_ptr);

int sscanf(const char *str, const char *format, ...)
{
  int n;
  va_list arg_ptr;
  va_start(arg_ptr, format);
  n=vsscanf(str,format,arg_ptr);
  va_end (arg_ptr);
  return n;
}
