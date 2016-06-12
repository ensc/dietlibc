#include "syscalls.h"

#ifdef __NR_parisc_fadvise64_64
syscall6(parisc_fadvise64_64, __parisc_fadvise64_64);
#endif
