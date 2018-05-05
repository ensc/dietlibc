#include <stdarg.h>
#include <unistd.h>
#include "dietstdio.h"

int __stdio_outs(const void *s,size_t len,void* cookie) __attribute__((weak));
int __stdio_outs(const void *s,size_t len,void* cookie) {
  (void)cookie;
  return (write(1,s,len)==(ssize_t)len)?1:0;
}

int vprintf(const char *format, va_list ap)
{
  struct arg_printf _ap = { 0, __stdio_outs };
  return __v_printf(&_ap,format,ap);
}

