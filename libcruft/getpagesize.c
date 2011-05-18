#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../dietelfinfo.h"
#include "../dietpagesize.h"

size_t __libc_getpagesize(void) {
#ifdef WANT_DYN_PAGESIZE
  static size_t	pgsz;

  if (__unlikely(pgsz==0)) {
    __diet_elf_addr_t	*v = __get_elf_aux_value(AT_PAGESZ);
    pgsz = *v;	/* causes segfault when 'v==NULL' */
  }

  return pgsz;
#else
  return __DIET_PAGE_SIZE_PREDEF;
#endif
}

size_t getpagesize(void)       __attribute__((weak,alias("__libc_getpagesize")));
