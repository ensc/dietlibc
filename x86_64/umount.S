#include "syscalls.h"

.text
.global umount
.type   umount,@function
umount:
        mov     $__NR_umount2,%rax
        xorq    %rsi,%rsi
        jmp     __unified_syscall
