#include "syscalls.h"
#include "dietfeatures.h"

#if !defined(__i386__) || !defined(WANT_I386_SOCKETCALL)

#ifdef __NR_setsockopt
syscall_weak(setsockopt,setsockopt,__libc_setsockopt)
#endif

#endif
