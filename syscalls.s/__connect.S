#include "syscalls.h"
#include "dietfeatures.h"

#if !defined(__i386__) || !defined(WANT_I386_SOCKETCALL)

#ifdef __NR_connect
syscall_weak(connect,connect,__libc_connect)
#endif

#endif
