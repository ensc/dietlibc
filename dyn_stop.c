#include "dietfeatures.h"

#ifdef WANT_DYNAMIC
typedef void(*structor)(void);

#if 0
/* dirty trick to force gcc to be in the data section (see dyn_start) */
static structor force_to_data[0] = {};

/* terminate the Construc/DestrucTORS with a NULL element */
asm ( ".section .ctors,\"aw\" ");
static structor __CTOR_END__[1]={((structor)0)};

asm ( ".section .dtors,\"aw\" ");
static structor __DTOR_END__[1]={((structor)0)};
#else
__attribute__((section(".ctors")))
static structor __CTOR_END__[1]={((structor)0)};

__attribute__((section(".dtors")))
static structor __DTOR_END__[1]={((structor)0)};
#endif

static void __do_global_ctors_aux(void)
{
  structor *cf=__DTOR_END__;	/* ugly trick to prevent warning */;
  for(cf=((__CTOR_END__)-1); (*cf) != (structor)-1; cf--) (*cf)();
}

__attribute__((section(".init"))) void _init()
{
  __do_global_ctors_aux();
}
#endif
