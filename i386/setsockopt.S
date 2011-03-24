#include <linuxnet.h>
#include <syscalls.h>

__socketcall(setsockopt,SETSOCKOPT)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
