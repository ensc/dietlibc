#include "syscalls.h"
#include "aarch64-features.h"

FUNC_START	futimesat
	cbz	x2, .LNULL
	/* x0 = dirfd, x1 = pathname, x2 = times */
	mov	x3, #1000
	ldp	x4, x5, [x2]
	mul	x5, x5, x3
	stp	x4, x5, [sp, #-32]!
	ldp	x4, x5, [x2, #16]
	mul	x5, x5, x3
	stp	x4, x5, [sp, #16]
.Lcont:
	mov	x2, sp
	mov	x3, #0
	mov	x8, #__NR_utimensat
	svc	#0
	add	sp, sp, #32
	b	__unified_syscall
.LNULL:
	/* if futimesat/utimes's times parameter is NULL, use current
	 * time, i.e. UTIME_NOW (0x3fffffff) in tv_nsec */
	mov	x4, 0
	mov	x5, 0x40000000
	sub	x5, x5, 1
	stp	x4, x5, [sp, #-16]!
	stp	x4, x5, [sp, #-16]!
	b	.Lcont
FUNC_END	futimesat
