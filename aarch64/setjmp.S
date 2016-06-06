#include "aarch64-features.h"

FUNC_START_WEAK	setjmp
FUNC_START	__setjmp
	mov	x1, #0
FUNC_END	__setjmp
FUNC_END	setjmp

FUNC_START_WEAK	sigsetjmp
FUNC_START	__sigsetjmp

	stp x19, x20, [x0, #0x00]
	stp x21, x22, [x0, #0x10]
	stp x23, x24, [x0, #0x20]
	stp x25, x26, [x0, #0x30]
	stp x27, x28, [x0, #0x40]

	stp x29, x30, [x0, #0x50]
	mov x2, sp
	str x2, [x0, #0x60]

	stp d8, d9, [x0, #0x68]
	stp d10, d11, [x0, #0x78]
	stp d12, d13, [x0, #0x88]
	stp d14, d15, [x0, #0x98]

	b	__sigjmp_save
FUNC_END	__sigsetjmp
FUNC_END	sigsetjmp
