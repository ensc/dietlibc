#include "dietfeatures.h"

#ifdef WANT_DYNAMIC
#include <stdlib.h>

typedef void(*structor)(void);

#if 0 

/* OLD tricks are this still needed ? */

/* dirty trick to force gcc to be in the data section */
static structor force_to_data[0] = {};

/* next dirty tricks to make program/sharedlib "local" objects and functions */

asm ( ".section .ctors,\"aw\" "); /* now change the section without gcc knowing it ! */
static structor __CTOR_LIST__[1]={((structor)-1)};

asm ( ".section .dtors,\"aw\" "); /* here too a section change */
static structor __DTOR_LIST__[1]={((structor)-1)};

#endif

__attribute__((section(".ctors")))
static structor __CTOR_LIST__[1]={((structor)-1)};

__attribute__((section(".dtors")))
static structor __DTOR_LIST__[1]={((structor)-1)};

static void __do_global_dtors_aux(void)
{
  structor *df=__CTOR_LIST__;	/* ugly trick to prevent warning */
  for (df=((__DTOR_LIST__)+1);(*df) != (structor)0; df++) (*df)();
}

__attribute__((section(".fini"))) void _fini(void);
__attribute__((section(".fini"))) void _fini(void)
{
  __do_global_dtors_aux();
}

/* pre main, post _start */
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
