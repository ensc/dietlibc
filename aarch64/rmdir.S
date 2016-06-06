#include "aarch64-features.h"

FUNC_START	rmdir
	mov	x2, #0x200		/* AT_REMOVEDIR */
	mov	x1, x0			/* pathname */
	mov	x0, #ASM_AT_FDCWD	/* olddirfd */
	b	unlinkat
FUNC_END	rmdir
