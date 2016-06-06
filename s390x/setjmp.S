#include <setjmp.h>

.text
.weak setjmp
.type setjmp,@function
setjmp:
.global __setjmp
.type	__setjmp,@function
__setjmp:
	lhi	%r3,1
	j	__sigsetjmp
.size	__setjmp,.-__setjmp
.size	setjmp,.-setjmp

.global _setjmp
.type	_setjmp,@function
_setjmp:
	lhi	%r3,0
.size	_setjmp,.-_setjmp


.global __sigsetjmp
.type	__sigsetjmp,@function
__sigsetjmp:
	stmg	%r6,%r15,0(%r2)
	std	%f8,80(%r2)
	std	%f9,88(%r2)
	std	%f10,96(%r2)
	std	%f11,104(%r2)
	std	%f12,112(%r2)
	std	%f13,120(%r2)
	std	%f14,128(%r2)
	std	%f15,136(%r2)

	/* Make a tail call to __sigjmp_save; it takes the same args.  */
	basr	%r1,0
#ifdef	PIC
.L0:	al	%r1,.L1-.L0(0,%r1)
	l	%r1,__sigjmp_save@GOT12(0,%r1)
#else
.L0:	l	%r1,.L1-.L0(0,%r1)
#endif
	br	%r1
	.p2align 3
#ifdef	PIC
.L1:	.long	_GLOBAL_OFFSET_TABLE_-.L0
#else
.L1:	.long	__sigjmp_save
#endif
.size __sigsetjmp,.-__sigsetjmp;
