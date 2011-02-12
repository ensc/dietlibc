#include "arm-features.h"

FUNC_START	__testandset
	mov	r2, r0
	mov	r1, #1
	swp	r0, r1, [r2]
	mov	pc, lr
FUNC_END	__testandset
