#include "dietfeatures.h"
#include "syscalls.h"
#include "dietwarning.h"

#ifdef __NR_sendfile64
#ifdef WANT_LARGEFILE_BACKCOMPAT
syscall(sendfile64,__dietlibc_sendfile64)
#else
syscall_weak(sendfile64,sendfile64,__libc_sendfile64)
#endif

link_warning(sendfile64,"sendfile64 is not portable")
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
