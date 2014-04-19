#ifndef __arm__
/* ARM needs special treatment for fadvise */

#include "syscalls.h"

#ifdef __NR_fadvise64
syscall(fadvise64,fadvise64)

#ifndef __NR_fadvise64_64
posix_fadvise = fadvise64
.globl posix_fadvise
#endif

#endif

#endif
