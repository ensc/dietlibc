#include <syscalls.h>

.text
.align 2
.weak __libc_waitpid
.global waitpid
waitpid:
	clr	$19
	br	wait4
