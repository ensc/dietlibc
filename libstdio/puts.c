#include <unistd.h>
#include <string.h>
#include "dietstdio.h"
#include "dietfeatures.h"

int __cheap_outs(const char *s,size_t len);
int __cheap_outs(const char *s,size_t len) {
  return (size_t)write(1,s,len)==len?1:0;
}

int __stdio_outs(const char *s,size_t len)  __attribute__((weak,alias("__cheap_outs")));

int puts(const char *s) {
  return (__stdio_outs(s,strlen(s)) && __stdio_outs("\n",1))?0:-1;
}

