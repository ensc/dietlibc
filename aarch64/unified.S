#include <dietfeatures.h>
#include "aarch64-features.h"

FUNC_START	__unified_syscall
	.hidden	__unified_syscall
	cmn x0, #4095
	b.cc 1f
	/* We rely on the fact that we assume that __errno_location
	 * will not modify r9 and r10 here. */
	neg x9, x0
#ifndef WANT_THREAD_SAFE
	ldr x0, =errno
#else
	mov x10, x30
	bl __errno_location
	mov x30, x10
#endif
	/* errno is int, so only store lowest 32bits, but perform
	 * the checks etc. on the entire 64bits, otherwise routines
	 * that return 64bit data types (e.g. read with ssize_t)
	 * will not properly catch the error */
	str w9, [x0]
	mov x0, #-1
1:
FUNC_END	__unified_syscall

/* here we go and "reuse" the return for weak-void functions */
#include "dietuglyweaks.h"

	ret
