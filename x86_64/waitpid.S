#include "syscalls.h"

.text
.type	waitpid,@function
.weak	waitpid
waitpid:
.type	__libc_waitpid,@function
.global __libc_waitpid
__libc_waitpid:
	mov	$0,%r10
	mov	$__NR_wait4,%rax
	call	__unified_syscall
	ret
