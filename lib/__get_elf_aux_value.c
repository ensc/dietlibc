#include "dietfeatures.h"

#ifdef WANT_ELFINFO
#include <stdlib.h>
#include "../dietelfinfo.h"

__diet_elf_addr_t const *__get_elf_aux_value(unsigned int tag)
{
  extern __diet_elf_addr_t const * const	__elfinfo;
  __diet_elf_addr_t const			*aux_ptr;

  for (aux_ptr = __elfinfo; aux_ptr[0]!=AT_NULL; aux_ptr += 2)
    if (aux_ptr[0]==tag)
      return aux_ptr+1;

  return NULL;
}

#endif
