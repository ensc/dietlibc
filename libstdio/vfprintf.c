#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "dietstdio.h"

int vfprintf(FILE *stream, const char *format, va_list arg_ptr)
{
  struct arg_printf ap = { stream, (int(*)(void*,size_t,size_t,void*)) fwrite };
  return __v_printf(&ap,format,arg_ptr);
}
