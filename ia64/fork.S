#include "syscalls.h"

.text
.globl fork
fork:
	mov r15 = __NR_clone
	mov r32 = 17
	br __unified_syscall
