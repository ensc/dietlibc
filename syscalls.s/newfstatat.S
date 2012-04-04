#include "syscalls.h"

#ifdef __NR_newfstatat
syscall(newfstatat,newfstatat)

#ifdef __NR_fstatat64
#  error __NR_newfstatat and __NR_fstatat64 must not be defined both
#endif

fstatat = newfstatat
.globl fstatat
#endif
