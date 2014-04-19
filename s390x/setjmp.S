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
	stm	%r6,%r15,0(%r2)
	std	%f4,40(%r2)
	std	%f6,48(%r2)

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
