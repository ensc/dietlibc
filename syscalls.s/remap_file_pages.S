#include "syscalls.h"

syscall(remap_file_pages,remap_file_pages)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
