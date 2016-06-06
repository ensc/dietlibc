#include "aarch64-features.h"

FUNC_START	utimes
	mov	x2, x1
	mov	x1, x0
	mov	x0, #ASM_AT_FDCWD
	b	futimesat
FUNC_END	utimes
