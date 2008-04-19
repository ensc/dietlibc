#include <stdio.h>
#include <sys/auxv.h>

static void* find_in_auxvec(long* x,long what) {
#ifndef WANT_ELFINFO
  while (*x) {
    if (*x==what)
      return (void*)x[1];
    x+=2;
  }
  return NULL;
#else
  __diet_elf_addr_t const	*a = __get_elf_aux_value(what);
  (void)x;
  return a ? (void *)*a : NULL;
#endif
}

#ifndef WANT_ELFINFO
static long* _auxvec;
#else
#define _auxvec	NULL
#endif

unsigned long getauxval(unsigned long type) {
  return (long)find_in_auxvec(_auxvec,type);
}

