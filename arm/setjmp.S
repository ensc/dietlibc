#include "arm-features.h"

FUNC_START_WEAK	setjmp
FUNC_START	__setjmp
	mov	r1, #0
FUNC_END	__setjmp
FUNC_END	setjmp

FUNC_START_WEAK	sigsetjmp
FUNC_START	__sigsetjmp

#ifdef DIET_HAVE_COPROC_REGS
	/* we have to work on a copy of 'r0' (jmpbuf *) */
	mov	ip, r0
#endif

#ifndef __SOFTFP__
# if __ARM_ARCH__ >= 7
	vstmia	ip!, {d0-d15}
#   ifdef __ARM_NEON__
	vstmia	ip!, {d16-d31}
#   endif
# else
	sfm	f4, 4, [ip]!
# endif
#endif

#ifdef __IWMMXT__
#  warning "setjmp will not save iwmmxt coprocessor registers"
#endif

#ifndef DIET_HAVE_COPROC_REGS
	stmia	r0, {r4-r11, sp, lr}
#else
	stmia	ip!, {r4-r11, sp, lr}
#endif

	b	__sigjmp_save
FUNC_END	__sigsetjmp
FUNC_END	sigsetjmp
