#include "syscalls.h"

.text
.global fork
fork:
	mov	2, %g1
	ta	0x10
	bcc,a	1f
	nop
	save	%sp, -96, %sp
#ifdef WANT_THREAD_SAVE
	call	__errno_location
	nop
#else
	sethi	%hi(errno), %o0
	or	%o0, %lo(errno), %o0
#endif
	st	%i0, [ %o0 ]
	retl
	restore	%g0, -1, %o0
1:	dec	%o1
	retl
	and	%o0, %o1, %o0
