#include "syscalls.h"

.text
.global pipe
pipe:
	mov	%o0, %o2
	mov	__NR_pipe, %g1
	ta 0x10
	bcc,a	1f
	nop
	save	%sp, -96, %sp
#ifdef WANT_THREAD_SAFE
	call	__errno_location
	nop
#else
	sethi	%hi(errno), %o0
	or	%o0, %lo(errno), %o0
#endif
	st	%i0, [ %o0 ]
	ret
	restore	%g0, -1, %o0

1:	st	%o0, [ %o2 ]
	st	%o1, [ %o2 + 4 ]
	retl
	mov	%g0, %o0
