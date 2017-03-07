#include "syscalls.h"
#include "dietfeatures.h"

#if !defined(__i386__) || !defined(WANT_I386_SOCKETCALL)
#ifdef __NR_socket
syscall_weak(socket,socket,__libc_socket)
#endif
#endif
