#include "syscalls.h"

.text
.weak fork
fork:
.global __libc_fork
__libc_fork:
	mov	2, %g1
	ta	0x6d
	bcc	%xcc, 1f
	nop
	save	%sp, -192, %sp
#ifdef WANT_THREAD_SAFE
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
