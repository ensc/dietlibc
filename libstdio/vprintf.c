#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "dietstdio.h"

int vprintf(const char *format, va_list arg_ptr)
{
  struct arg_printf ap = { stdout, (int(*)(void*,size_t,size_t,void*)) fwrite };
  return __v_printf(&ap,format,arg_ptr);
}
