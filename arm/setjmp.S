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
	stmia	r0, {r4-r11, sp, lr}
#ifndef	__SOFTFP__
	sfm	f4, 4, [r0, #10*4]
#endif
#ifdef __IWMMXT__
#  warning "setjmp will not save iwmmxt coprocessor registers"
#endif
	b	__sigjmp_save

