#include "arm-features.h"

FUNC_START	__longjmp
	mov	ip, r0
	movs	r0, r1
	moveq	r0, #1

#ifndef __SOFTFP__
# if __ARM_ARCH__ == 7
	vldm	ip!, {d0-d15}
#   ifdef __ARM_NEON__
	vldm	ip!, {d16-d31}
#   endif
# else
	lfm	f4, 4, [ip]!
# endif
#endif

#ifdef __IWMMXT__
#  warning "sigjmp will not restore iwmmxt coprocessor registers"
#endif

	ldmia   ip!, {r4-r11, sp, pc}
FUNC_END	__longjmp
