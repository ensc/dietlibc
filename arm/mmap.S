#include "arm-features.h"

#if !defined(__NR_mmap) && !defined(__ARM_EABI__)

#include <errno.h>
#include "syscalls.h"
#include "dietfeatures.h"
#include "arm-features.h"

.text

@
@ mmap takes 6 parameters - ie more than can be passed in registers via the
@ regular syscall interface. Instead, parameters are passed on the stack.
@
@ On entry, the compiler will have already placed the fifth and sixth
@ parameters on the stack - all we need do here is push the first four and
@ call the syscall.
@

FUNC_START	mmap
	stmdb	sp!, {r0, r1, r2, r3}
	mov	r0, sp
	swi	__NR_mmap
	add	sp, sp, #16
	b	__unified_syscall
FUNC_END	mmap

#endif
