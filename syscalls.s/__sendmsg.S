#include "syscalls.h"
#include "dietfeatures.h"

#if !defined(__i386__) || !defined(WANT_I386_SOCKETCALL)

#ifdef __NR_sendmsg
syscall_weak(sendmsg,sendmsg,__libc_sendmsg)
#endif

#endif
