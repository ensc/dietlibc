#include <dietfeatures.h>

.text
.global __unified_syscall
__unified_syscall:
	ta	0x10

	bcc	1f
	save	%sp, -104, %sp

	neg	%i0, %i0
1:
	add	%i0, 0xff, %l2
	cmp	%l2, 0xfe
	bgu	2f
	neg	%i0, %l3
#ifdef WANT_THREAD_SAFE
	call	__errno_location
	nop
#else
	sethi	%hi(errno), %o0
	or	%o0, %lo(errno), %o0
#endif
	st	%l3, [ %o0 ]
	mov	-1, %i0
2:
	ret
	restore
