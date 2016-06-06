#include "aarch64-features.h"

FUNC_START	rename
	mov	x3, x1			/* newpath */
	mov	x1, x0			/* oldpath */
	mov	x2, #ASM_AT_FDCWD	/* newdirfd */
	mov	x0, #ASM_AT_FDCWD	/* olddirfd */
	b	renameat
FUNC_END	rename
