#include <stdarg.h>
#include <sys/types.h>
#include <stdlib.h>
#include "dietstdio.h"
#include <unistd.h>

int vscanf(const char *format, va_list arg_ptr)
{
  struct arg_scanf farg = { (void*)stdin, (int(*)(void*))fgetc, (int(*)(int,void*))ungetc };
  return __v_scanf(&farg,format,arg_ptr);
}
