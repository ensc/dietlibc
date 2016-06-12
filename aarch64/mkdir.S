#include "aarch64-features.h"

FUNC_START	mkdir
	mov	x2, x1			/* mode */
	mov	x1, x0			/* pathname */
	mov	x0, #ASM_AT_FDCWD	/* olddirfd */
	b	mkdirat
FUNC_END	mkdir
