#include "aarch64-features.h"

FUNC_START	symlink
	mov	x2, x1			/* linkpath */
	mov	x1, #ASM_AT_FDCWD	/* olddirfd */
	b	symlinkat
FUNC_END	symlink
