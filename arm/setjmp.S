.text
.weak setjmp
setjmp:
.global __setjmp
__setjmp:
	mov	r1, #0
.global __sigsetjmp
__sigsetjmp:
.weak sigsetjmp
sigsetjmp:
	sfm     f4, 4, [r0], #48
	stmia   r0, {r4, r5, r6, r7, r8, r9, sl, fp, sp, lr}
/*	stmia	r0, {v1-v6, sl, fp, sp, lr} */
	sub     r0, r0, #30
	b	__sigjmp_save

