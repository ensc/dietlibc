#include <linuxnet.h>
#include <syscalls.h>

__socketcall(shutdown,SHUTDOWN)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
