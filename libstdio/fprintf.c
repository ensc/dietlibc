#include <stdarg.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

extern int vsnprintf (char *str,size_t size,const char *format, va_list arg_ptr);

int fprintf(FILE *f,const char *format,...) {
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
  va_end (arg_ptr);
  fwrite(printf_buf,n,1,f);
  return n;
}
