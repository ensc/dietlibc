#include "syscalls.h"

#ifdef __NR_inotify_add_watch
syscall(inotify_add_watch,inotify_add_watch)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
