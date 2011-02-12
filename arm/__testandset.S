#include "arm-features.h"

FUNC_START	__testandset
	mov	r2, r0
	mov	r1, #1
# if __ARM_ARCH__ < 6
	swp	r0, r1, [r2]
# else
1:	ldrex	r0, [r2]
	strex	r3, r1, [r2]
	cmp	r3, #0
	bne	1b
# endif
	RET
FUNC_END	__testandset
