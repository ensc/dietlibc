#include "syscalls.h"
#include "dietwarning.h"

#ifdef __NR_sendfile64
syscall_weak(sendfile64,sendfile64,__libc_sendfile64)

link_warning(sendfile64,"sendfile64 is not portable")
#endif
