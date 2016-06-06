#include "aarch64-features.h"

/* getdents() is not supported on aarch64, and we don't want to emulate it
 * with the totally different structure layout. getdents64() is available
 * and our readdir{,_r}() implementation will use getdents64 on aarch64. */
FUNC_START	getdents
	mov	x0, #-38 /* ENOSYS */
	b	__unified_syscall
FUNC_END	getdents
