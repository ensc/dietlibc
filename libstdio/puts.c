#include <unistd.h>
#include <string.h>
#include "dietstdio.h"
#include "dietfeatures.h"

int __cheap_outs(const char *s,int len) {
  return write(1,s,len)==len?1:0;
}

int __buffered_outs(const char *s,int len)  __attribute__((weak,alias("__cheap_outs")));

int puts(const char *s) {
  return (__buffered_outs(s,strlen(s)) && __buffered_outs("\n",1))?0:-1;
}

