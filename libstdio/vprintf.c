#include <stdarg.h>
#include <unistd.h>
#include "dietstdio.h"

__attribute__((weak)) int __buffered_outs(const char *s,size_t len) {
  return (write(1,s,len)==(int)len)?1:0;
}

int vprintf(const char *format, va_list ap)
{
  struct arg_printf _ap = { 0, (int(*)(void*,size_t,void*)) __buffered_outs };
  return __v_printf(&_ap,format,ap);
}

