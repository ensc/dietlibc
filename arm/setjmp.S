#include "arm-features.h"

FUNC_START_WEAK	setjmp
FUNC_START	__setjmp
	mov	r1, #0
FUNC_END	__setjmp
FUNC_END	setjmp

FUNC_START_WEAK	sigsetjmp
FUNC_START	__sigsetjmp
	stmia	r0, {r4-r11, sp, lr}
#ifndef	__SOFTFP__
	sfm	f4, 4, [r0, #10*4]
#endif
#ifdef __IWMMXT__
#  warning "setjmp will not save iwmmxt coprocessor registers"
#endif
	b	__sigjmp_save
FUNC_END	__sigsetjmp
FUNC_END	sigsetjmp

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
