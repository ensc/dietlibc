#include "syscalls.h"

#ifdef __NR_fadvise64
syscall(fadvise64,fadvise64)

#ifndef __NR_fadvise64_64
.set   posix_fadvise, fadvise64
.globl posix_fadvise
#endif

#endif
