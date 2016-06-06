#include "aarch64-features.h"

FUNC_START	__longjmp
	ldp x19, x20, [x0, #0x00]
	ldp x21, x22, [x0, #0x10]
	ldp x23, x24, [x0, #0x20]
	ldp x25, x26, [x0, #0x30]
	ldp x27, x28, [x0, #0x40]

	ldp x29, x30, [x0, #0x50]
	ldp d8, d9, [x0, #0x68]
	ldp d10, d11, [x0, #0x78]
	ldp d12, d13, [x0, #0x88]
	ldp d14, d15, [x0, #0x98]

	ldr x2, [x0, #0x60]
	mov sp, x2

	cmp x1, #0
	mov x0, #1
	csel x0, x1, x0, ne

	/* according to glibc's source, ret will mispredict, hence use br instead */
	br	x30
FUNC_END	__longjmp
