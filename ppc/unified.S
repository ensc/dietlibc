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
	stwu	1,-16(1)
	mflr	0
	stw	0,20(1)
	stw	3,12(1)
	bl	__errno_location
	lwz	0,12(1)
	stw	0,0(3)
	lwz	0,20(1)
	mtlr	0
	addi	1,1,16
#else
	lis	9,errno@ha
	stw	3,errno@l(9)
#endif
	li	0,-1
.L1:
	mr	3,0
	blr

