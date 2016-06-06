#include "aarch64-features.h"

FUNC_START	lchown
	mov	x4, #0x100	/* AT_SYMLINK_NOFOLLOW */
	mov	x3, x2
	mov	x2, x1
	mov	x1, x0
	mov	x0, #ASM_AT_FDCWD
	b	fchownat
FUNC_END	lchown
