#include "aarch64-features.h"

FUNC_START	stat
	mov	x3, #0			/* flags */
	mov	x2, x1			/* buf */
	mov	x1, x0			/* pathname */
	mov	x0, #ASM_AT_FDCWD	/* olddirfd */
	b	fstatat
FUNC_END	stat
