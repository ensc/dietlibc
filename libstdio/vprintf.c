#include <stdarg.h>
#include "dietstdio.h"

static int __aaa_cheap_outs(const char *s,int len) {
  return write(1,s,len)==len?1:0;
}

int __buffered_outs(const char *s,int len)  __attribute__((weak,alias("__aaa_cheap_outs")));

int vprintf(const char *format, va_list ap)
{
  struct arg_printf _ap = { 0, (int(*)(void*,size_t,void*)) __buffered_outs };
  return __v_printf(&_ap,format,ap);
}

