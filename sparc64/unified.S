#include <dietfeatures.h>
#include "syscalls.h"

.text
.weak exit
.type exit,function
exit:
.global _exit
.type	_exit,function
_exit:
	mov	__NR_exit, %g1
.global __unified_syscall
__unified_syscall:
	ta	0x6d

	bcc	%xcc, 1f	/* no error ? */
	nop

	save	%sp, -192, %sp
	call	__errno_location
	nop
	st	%i0, [ %o0 ]	/* store errno */
	ret
	restore %g0, -1, %o0	/* return -1 */
1:
	retl
	nop

