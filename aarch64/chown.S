#include "aarch64-features.h"

FUNC_START	chown
	mov	x4, #0
	mov	x3, x2
	mov	x2, x1
	mov	x1, x0
	mov	x0, #ASM_AT_FDCWD
	b	fchownat
FUNC_END	chown
