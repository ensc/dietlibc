#include "aarch64-features.h"
#include "syscalls.h"

FUNC_START	poll
	mov	x3, #0 /* tv_nsec, and later sigmask pointer */
	/* store timeout in struct timespec */
	stp	x2, x3, [sp, #-16]!
	mov	x2, sp
	mov	x8, #__NR_ppoll
	svc	#0
	add	sp, sp, #16
	b __unified_syscall
FUNC_END	poll
