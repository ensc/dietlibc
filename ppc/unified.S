#include <dietfeatures.h>

.text
.global __unified_syscall
__unified_syscall:
	sc
	mfcr	0
	andis.	9,0,4096
	mr	0,3
	beq	.L1
#ifdef WANT_THREAD_SAVE
	bl	__set_errno
#else
	lis	9,errno@ha
	stw	3,errno@l(9)
#endif
	li	0,-1
.L1:
	mr	3,0
	blr

