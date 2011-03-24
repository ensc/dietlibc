#include <linuxnet.h>
#include <syscalls.h>

__socketcall(getsockopt,GETSOCKOPT)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
