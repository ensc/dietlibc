#include "dietfeatures.h"
#include "syscalls.h"

#ifdef __NR_ftruncate64
#ifdef WANT_LARGEFILE_BACKCOMPAT
syscall_weak(ftruncate64,__dietlibc_ftruncate64_syscall,__dietlibc_ftruncate64)
#else
syscall(ftruncate64,ftruncate64)
#endif
#endif
