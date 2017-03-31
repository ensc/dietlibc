#include "syscalls.h"
#include "dietfeatures.h"

#if !defined(__i386__) || !defined(WANT_I386_SOCKETCALL)

#ifdef __NR_sendto
syscall_weak(sendto,sendto,__libc_sendto)
#endif

#endif
