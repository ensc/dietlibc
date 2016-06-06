#include <setjmp.h>

.text
.global __longjmp
.type __longjmp,@function
__longjmp:
	lmg	%r6,%r15,0(%r2)
	ld	%f8,80(%r2)
	ld	%f9,88(%r2)
	ld	%f10,96(%r2)
	ld	%f11,104(%r2)
	ld	%f12,112(%r2)
	ld	%f13,120(%r2)
	ld	%f14,128(%r2)
	ld	%f15,136(%r2)
	chi	%r3,0
	jne	1f
	lhi	%r3,1
1:	lr	%r2,%r3
	br	%r14
.size __longjmp,.-__longjmp;
