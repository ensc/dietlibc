#include "syscalls.h"

.text
.type	waitpid,@function
.weak	waitpid
waitpid:
.type	__libc_waitpid,@function
.global __libc_waitpid
__libc_waitpid:
	mov	$0,%rcx
	mov	$__NR_wait4,%al
	jmp	__unified_syscall
