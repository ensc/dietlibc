#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "dietstdio.h"

static int __fwrite(const void*ptr, size_t nmemb, void* cookie) {
  return write((uintptr_t)cookie,ptr,nmemb);
}

int vfdprintf(int fd, const char *format, va_list arg_ptr)
{
  struct arg_printf ap = { (void*)(long)fd, __fwrite };
  return __v_printf(&ap,format,arg_ptr);
}
