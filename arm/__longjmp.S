#include "arm-features.h"

FUNC_START	__longjmp
	mov	ip, r0
	movs	r0, r1
	moveq	r0, #1
#ifndef	__SOFTFP__
	lfm	f4, 4, [ip], #48
#endif
	ldmia   ip, {r4-r11, sp, pc}
FUNC_END	__longjmp
