.text
.global __longjmp
__longjmp:
	mov	ip, r0
	movs	r0, r1
	moveq	r0, #1
#ifndef	__SOFTFP__
	lfm	f4, 4, [ip], #48
#endif
	ldmia   ip, {r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}
