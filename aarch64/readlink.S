#include "aarch64-features.h"

FUNC_START	readlink
	mov	x3, x2			/* bufsize */
	mov	x2, x1			/* buf */
	mov	x1, x0			/* pathname */
	mov	x0, #ASM_AT_FDCWD	/* olddirfd */
	b	readlinkat
FUNC_END	readlink
