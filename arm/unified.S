#include <dietfeatures.h>

.text
.global __unified_syscall
__unified_syscall:
	ldmfd	sp!, {r4, r5, r6}
	cmn	r0, #4096
	movcc	pc, lr
	rsb	r0, r0, #0
#ifdef WANT_THREAD_SAFE
	stmdb	sp!, {r0, lr}
	bl	__errno_location
	ldmia	sp!, {r1, lr}
	str	r1, [r0]
#else
	ldr	r1, .L0
	str	r0, [r1]
#endif
	mvn	r0, #0

/* here we go an "reused" the return for weak-void functions */
#include "dietuglyweaks.h"

	mov	pc, lr

#ifndef WANT_THREAD_SAFE
.L0:
	.long errno
#endif
