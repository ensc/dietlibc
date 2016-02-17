#include "syscalls.h"

.text
.global umount
#ifdef __PIE__
.hidden umount
#endif
.type   umount,@function
umount:
        mov     $__NR_umount2,%al
        xorq    %rsi,%rsi
        jmp     __unified_syscall
