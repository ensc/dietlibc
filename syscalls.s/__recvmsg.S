#include "syscalls.h"
#include "dietfeatures.h"

#if !defined(__i386__) || !defined(WANT_I386_SOCKETCALL)

#ifdef __NR_recvmsg
syscall_weak(recvmsg,recvmsg,__libc_recvmsg)
#endif

#endif
