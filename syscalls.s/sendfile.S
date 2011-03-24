#include "syscalls.h"
#include "dietwarning.h"

syscall_weak(sendfile,sendfile,__libc_sendfile)

link_warning(sendfile,"sendfile is not portable")

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
