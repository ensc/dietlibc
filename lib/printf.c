#include <stdarg.h>
#include <linux/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "dietstdio.h"

int vsnprintf (char *str,size_t size,const char *format, va_list arg_ptr);

int printf(const char *format,...)
{
  int n;
  char *printf_buf;
/*  char printf_buf[1024]; */
  va_list arg_ptr;
  va_start(arg_ptr, format);
  n=vsnprintf(0,1000000,format,arg_ptr);
/*  write(1,printf_buf,strlen(printf_buf)); */
  va_end (arg_ptr);
  va_start (arg_ptr, format);
  printf_buf=alloca(n+2);
  n=vsnprintf(printf_buf,n+1,format,arg_ptr);
#ifdef WANT_BUFFERED_STDIO
  fwrite(printf_buf,n,1,stdout);
#else
  write(1,printf_buf,n);
#endif
  va_end (arg_ptr);
  return n;
}
