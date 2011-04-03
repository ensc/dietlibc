#include "arm-features.h"

FUNC_START_WEAK	waitpid
FUNC_START	__libc_waitpid
	mov	r3, #0
	b	wait4
FUNC_END	__libc_waitpid
FUNC_END	waitpid
