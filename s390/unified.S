#include <dietfeatures.h>

.text
.global __unified_syscall
__unified_syscall:
	lhi	%r4,-4095
	clr	%r2,%r4
	jm	.Lnoerror

#ifdef WANT_THREAD_SAFE
	stm	%r11,%r15,44(%r15)
	lr	%r0,%r15
	ahi	%r15,-96
	st	%r0,0(%r15)

	lcr	%r11,%r2
	basr	%r13,%r0
	l	%r3,[.L0-.](%r13)
	basr	%r14,%r3
	st	%r11,0(%r2)

	l	%r15,0(%r15)
	lm	%r11,%r15,44(%r15)
#else
	basr	%r1,%r0
	l	%r3,[.L0-.](%r1)
	st	%r2,0(%r3)
#endif
	lhi	%r2,-1

.Lnoerror:

/* here we go and "reuse" the return for weak-void functions */
#include "dietuglyweaks.h"

	br	%r14

.align 4
.L0:
#ifdef WANT_THREAD_SAFE
	.long	__errno_location
#else
	.long	errno
#endif
