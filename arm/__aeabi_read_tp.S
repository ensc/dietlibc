#include "arm-features.h"

FUNC_START __aeabi_read_tp

#if __ARM_ARCH__ < 6
	mvn     r0, #0xf000
	sub     pc, r0, #31
#else
	mrc     15, 0, r0, cr13, cr0, 3
	bx      lr
#endif

FUNC_END __aeabi_read_tp
