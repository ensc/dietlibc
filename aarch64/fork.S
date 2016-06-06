#include "syscalls.h"
#include "aarch64-features.h"

FUNC_START_WEAK	fork
FUNC_START	__libc_fork
	mov	x0, #17 /* flags = SIGCHLD */
	mov	x1, sp  /* child stack */
	mov	x2, #0  /* ptid */
	mov	x3, #0  /* tls */
	mov	x4, #0  /* ctid */
	mov	x8, #__NR_clone
	svc	#0
	b	__unified_syscall
FUNC_END	__libc_fork
FUNC_END	fork
