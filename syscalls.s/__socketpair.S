#include "syscalls.h"
#include "dietfeatures.h"

#if !defined(__i386__) || !defined(WANT_I386_SOCKETCALL)

#ifdef __NR_socketpair
syscall_weak(socketpair,socketpair,__libc_socketpair)
#endif

#endif
