#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "dietstdio.h"

struct str_data {
  unsigned char* str;
  size_t len;
  size_t size;
};

static int swrite(void*ptr, size_t allways1, size_t nmemb, struct str_data* sd) {
  size_t tmp=sd->size-sd->len;
  if (tmp>0) {
    size_t len=nmemb;
    if (len>tmp) len=tmp;
    memcpy(&sd->str[sd->len],ptr,len);
    sd->len+=len;
    sd->str[sd->len]=0;
  }
  return nmemb;
}

int vsnprintf(char* str, size_t size, const char *format, va_list arg_ptr) {
  struct str_data sd = { str, 0, size-1 };
  struct arg_printf ap = { &sd, (int(*)(void*,size_t,size_t,void*)) swrite };
  return __v_printf(&ap,format,arg_ptr);
}
