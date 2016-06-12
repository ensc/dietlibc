#include "aarch64-features.h"

FUNC_START	unlink
	mov	x2, #0			/* flags */
	mov	x1, x0			/* pathname */
	mov	x0, #ASM_AT_FDCWD	/* olddirfd */
	b	unlinkat
FUNC_END	unlink
