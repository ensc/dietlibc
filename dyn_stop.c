#include "dietfeatures.h"

#ifdef WANT_DYNAMIC
typedef void(*structor)(void);

__attribute__((section(".ctors")))
static structor __CTOR_END__[1]={((structor)0)};

__attribute__((section(".dtors")))
static structor __DTOR_END__[1]={((structor)0)};

static void __do_global_ctors_aux(void)
{
  structor *cf=__DTOR_END__;	/* ugly trick to prevent warning */
  for(cf=((__CTOR_END__)-1); (*cf) != (structor)-1; cf--) (*cf)();
}

void _init(void) __attribute__((section(".init")));
__attribute__((section(".init"))) void _init(void)
{
  __do_global_ctors_aux();
}
#endif
