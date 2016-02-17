#include "syscalls.h"

.text
.type	waitpid,@function
.weak	waitpid
waitpid:
.type	__libc_waitpid,@function
.global __libc_waitpid
#ifdef __PIE__
.hidden __libc_waitpid
.hidden waitpid
#endif
__libc_waitpid:
	xor	%rcx,%rcx
	mov	$__NR_wait4,%al
	jmp	__unified_syscall
