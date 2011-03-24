#include "syscalls.h"

#ifdef __NR_inotify_init
syscall(inotify_init,inotify_init)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
