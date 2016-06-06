#include "syscalls.h"

#ifdef __NR_fadvise64_64
syscall(fadvise64_64,__ppc_fadvise64_64)
#endif
