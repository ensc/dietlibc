#include "syscalls.h"
#include "dietfeatures.h"

#if !defined(__i386__) || !defined(WANT_I386_SOCKETCALL)

#ifdef __NR_getsockname
syscall_weak(getsockname,getsockname,__libc_getsockname)
#endif

#endif
