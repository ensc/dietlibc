#include <dietfeatures.h>

.text
.global __unified_syscall
__unified_syscall:
	sc
	mfcr	0
	andis.	9,0,4096
	mr	0,3
	beq	.L1
#ifdef WANT_THREAD_SAFE
	mr.	5,3
	mflr	4
	bl	__errno_location
	stw	5,0(3)
	mtlr	4
#else
	lis	9,errno@ha
	stw	3,errno@l(9)
#endif
	li	0,-1
.L1:
	mr	3,0
	blr

