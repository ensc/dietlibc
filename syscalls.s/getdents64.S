#include "dietfeatures.h"
#include "syscalls.h"

#ifdef __NR_getdents64
#ifdef WANT_LARGEFILE_BACKCOMPAT
syscall(getdents64,__dietlibc_getdents64)
#else
syscall(getdents64,getdents64)
#endif
#endif
