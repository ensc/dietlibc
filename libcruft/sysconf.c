#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <sys/resource.h>

#include "dietelfinfo.h"
#include "dietpagesize.h"

extern int __sc_nr_cpus();

long sysconf(int name)
{
  switch(name)
  {
  case _SC_OPEN_MAX:
    {
      struct rlimit limit;
      getrlimit(RLIMIT_NOFILE, &limit);
      return limit.rlim_cur;
    }
  case _SC_CLK_TCK:
#ifdef WANT_ELFINFO
    {
      __diet_elf_addr_t	*v = __get_elf_aux_value(AT_CLKTCK);
      if (v)
	return *v;
    }
#endif

#ifdef __alpha__
    return 1024;
#else
    return 100;
#endif

  case _SC_PAGESIZE:
    return __libc_getpagesize();

  case _SC_ARG_MAX:
    return ARG_MAX;

  case _SC_NGROUPS_MAX:
    return NGROUPS_MAX;

  case _SC_NPROCESSORS_ONLN:
    return __sc_nr_cpus();

  }
  return -1;
}
