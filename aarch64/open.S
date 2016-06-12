#include "aarch64-features.h"

FUNC_START_WEAK	open
FUNC_START	__libc_open
	mov	x3, x2			/* mode (maybe ignored) */
	mov	x2, x1			/* flags */
	mov	x1, x0			/* pathname */
	mov	x0, #ASM_AT_FDCWD	/* olddirfd */
	b	openat
FUNC_END	__libc_open
FUNC_END	open
