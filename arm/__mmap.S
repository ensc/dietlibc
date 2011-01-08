#ifndef __NR_mmap

#include <errno.h>
#include "syscalls.h"
#include "arm-features.h"

@
@ mmap takes 6 parameters - ie more than can be passed in registers via the
@ regular syscall interface. Instead, parameters are passed on the stack.
@
@ On entry, the compiler will have already placed the fifth and sixth
@ parameters on the stack - all we need do here is push the first four and
@ call the syscall.
@

@ TODO: check whether pgoffset is really in 4096 bytes or in PAGESIZE
@	units.

FUNC_START	mmap
        stmfd	sp!,{r4,r5,r7,lr}
	LOAD_ARG4_5

#ifdef __ARM_EABI__
	ldr	r7, =__NR_mmap2
#endif

	tst	r5, #0x0ff	@ check whether r5 % 4096 == 0
	tsteq	r5, #0xf00	@ holds
	movne	r0, #-EINVAL
#ifdef __ARM_EABI__
	bne	__unified_syscall
#else
	bne	__unified_syscall4
#endif

	lsr	r5, r5, #12	@ div-by 4096
	SWI_UNIFIED4
FUNC_END	mmap

#endif
