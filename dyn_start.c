#include "dietfeatures.h"

#ifdef WANT_DYNAMIC
#include <stdlib.h>

typedef void(*structor)(void);

__attribute__((section(".ctors")))
static structor __CTOR_LIST__[1]={((structor)-1)};

__attribute__((section(".dtors")))
static structor __DTOR_LIST__[1]={((structor)-1)};

static void __do_global_dtors_aux(void)
{
  structor *df=__CTOR_LIST__;	/* ugly trick to prevent warning */
  for (df=((__DTOR_LIST__)+1);(*df) != (structor)0; df++) (*df)();
}

void _fini(void) __attribute__((section(".fini")));
__attribute__((section(".fini"))) void _fini(void)
{
  __do_global_dtors_aux();
}

#ifndef __DYN_LIB_SHARED
/* pre main, post _start */
int _dyn_start(int argc, char **argv, char **envp, structor dl_init);
int _dyn_start(int argc, char **argv, char **envp, structor dl_init)
{
  static __attribute__((section(".init"))) void _init(void);
  int main(int argc, char **argv, char **envp);

  if (dl_init) atexit(dl_init);
  _init();
  atexit(_fini);
  return main(argc, argv, envp);
}
#endif
#endif
