#include "syscalls.h"
#include "dietfeatures.h"

#if !defined(__i386__) || !defined(WANT_I386_SOCKETCALL)

#ifdef __NR_listen
syscall_weak(listen,listen,__libc_listen)
#endif

#endif
