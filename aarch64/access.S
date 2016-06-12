#include "aarch64-features.h"

FUNC_START	access
	mov	x3, #0
	mov	x2, x1
	mov	x1, x0
	mov	x0, #ASM_AT_FDCWD
	b	faccessat
FUNC_END	access
