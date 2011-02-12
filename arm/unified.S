
#include <dietfeatures.h>
#include "arm-features.h"

#ifdef __ARM_EABI__
FUNC_START	__unified_syscall4
FUNC_START	__unified_syscall
	.hidden	__unified_syscall
        cmn     r0, #4096
        rsbcs   r2, r0, #0
        ldrcs   r3, 1f
        mvncs   r0, #0
        strcs   r2, [r3]
        ldmfd   sp!,{r4,r5,r7,pc}
	.balign 4
1:
        .word   errno
FUNC_END	__unified_syscall
FUNC_END	__unified_syscall4
	
/* here we go and "reuse" the return for weak-void functions */
#include "dietuglyweaks.h"

	mov	pc, lr			@ return

#else	
	
FUNC_START	__unified_syscall4
	ldmia	sp!, {r4, r5, r6}

FUNC_START	__unified_syscall
	cmn	r0, #4096
	movcc	pc, lr			@ return value comes direct from kernel.

	rsb	r1, r0, #0
#ifdef WANT_THREAD_SAFE
	stmdb	sp!, {r1, lr}
	bl	__errno_location
	ldmia	sp!, {r1, lr}
#else
	ldr	r0, .L0
#endif
	str	r1, [r0]		@ store error code into 'errno'
	mvn	r0, #0			@ return value = -1

/* here we go and "reuse" the return for weak-void functions */
#include "dietuglyweaks.h"

	mov	pc, lr			@ return

#ifndef WANT_THREAD_SAFE
	.align	2
.L0:	.long 	errno
#endif
FUNC_END	__unified_syscall
FUNC_END	__unified_syscall4

#endif
