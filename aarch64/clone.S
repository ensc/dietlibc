#include "syscalls.h"
#include "dietfeatures.h"

#include <errno.h>

FUNC_START	clone
	/* sanity checks */
	cbz	x0, .Linval
	cbz	x1, .Linval
	/* put function pointer, arg pointer on the new stack */
	stp	x0, x3, [x1, #-16]!
	mov	x0, x2 /* flags */
	mov	x2, x4 /* ptid */
	mov	x3, x5 /* tls */
	mov	x4, x6 /* ctid */
	mov	x8, #__NR_clone
	svc	#0
	cbz	x0, .Lchild
	b	__unified_syscall
.Linval:
	mov	x0, #-EINVAL
	b	__unified_syscall

.Lchild:
	mov	x29, #0
	ldp	x1, x0, [sp], #16
	blr	x1
	b	exit
FUNC_END	clone
