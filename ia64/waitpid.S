#include <syscalls.h>

	.text
	.weak waitpid
waitpid:
	.global __libc_waitpid
__libc_waitpid:
	alloc	r2=ar.pfs,4,0,0,0
	mov	in3=r0
	mov	r15=__NR_wait4
	br	__unified_syscall
