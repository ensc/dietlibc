#include "arm-features.h"

FUNC_START_WEAK	setjmp
FUNC_START	__setjmp
	mov	r1, #0
FUNC_END	__setjmp
FUNC_END	setjmp

FUNC_START_WEAK	sigsetjmp
FUNC_START	__sigsetjmp
#ifndef	__SOFTFP__
	sfm	f4, 4, [r0], #48
#endif
	stmia	r0, {r4-r11, sp, lr}
	sub	r0, r0, #48
	b	__sigjmp_save
FUNC_END	__sigsetjmp
FUNC_END	sigsetjmp
