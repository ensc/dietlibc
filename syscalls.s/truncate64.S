#include "dietfeatures.h"
#include "syscalls.h"

#ifdef __NR_truncate64
#ifdef WANT_LARGEFILE_BACKCOMPAT
syscall_weak(truncate64,__dietlibc_truncate64_syscall,__dietlibc_truncate64)
#else
syscall(truncate64,truncate64)
#endif
#endif
