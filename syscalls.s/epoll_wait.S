#include "syscalls.h"

#ifdef __NR_epoll_wait
syscall(epoll_wait,epoll_wait)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
