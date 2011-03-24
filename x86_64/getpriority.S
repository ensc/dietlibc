#include "syscalls.h"

.text
.global getpriority
.type   getpriority,@function
getpriority:
     mov     $__NR_getpriority,%al
     xorq    %rsi,%rsi
     call    __unified_syscall
     neg     %rax    /* the value is bios by 20 to avoid negative values */
     add     $20,%rax
     ret

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
