#include "syscalls.h"

#ifdef __NR__llseek
.global llseek
.type	llseek,function
llseek:
syscall(_llseek,_llseek)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
