#include "aarch64-features.h"

/* ustat() is deprecated, statfs() should be used */
FUNC_START	ustat
	mov	x0, #-38 /* ENOSYS */
	b	__unified_syscall
FUNC_END	ustat
