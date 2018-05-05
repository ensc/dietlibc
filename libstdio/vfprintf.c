#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "dietstdio.h"

static int __fwrite(const void*ptr, size_t nmemb, void* cookie) {
  return fwrite(ptr,1,nmemb,(FILE*)cookie);
}

int vfprintf(FILE *stream, const char *format, va_list arg_ptr)
{
  struct arg_printf ap = { stream, __fwrite };
  return __v_printf(&ap,format,arg_ptr);
}
