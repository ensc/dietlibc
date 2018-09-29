#include <stdio.h>
#include <sys/auxv.h>

static void* find_in_auxvec(long* x,long what) {
  while (*x) {
    if (*x==what)
      return (void*)x[1];
    x+=2;
  }
  return NULL;
}

long* _auxvec;

unsigned long getauxval(unsigned long type) {
  return (long)find_in_auxvec(_auxvec,type);
}

