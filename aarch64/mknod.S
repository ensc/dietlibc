#include "aarch64-features.h"

FUNC_START	mknod
	mov	x3, x2			/* dev */
	mov	x2, x1			/* mode */
	mov	x1, x0			/* pathname */
	mov	x0, #ASM_AT_FDCWD	/* olddirfd */
	b	mknodat
FUNC_END	mknod
