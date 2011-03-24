#include "syscalls.h"

#ifdef __NR_epoll_create
syscall(epoll_create,epoll_create)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
