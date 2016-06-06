#include "aarch64-features.h"

FUNC_START	link
	mov	x4, #0			/* flags */
	mov	x3, x1			/* newpath */
	mov	x1, x0			/* oldpath */
	mov	x2, #ASM_AT_FDCWD	/* newdirfd */
	mov	x0, #ASM_AT_FDCWD	/* olddirfd */
	b	linkat
FUNC_END	link
