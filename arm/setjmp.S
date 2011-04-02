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

#ifdef DIET_HAVE_COPROC_REGS
	/* we have to work on a copy of 'r0' (jmpbuf *) */
	mov	ip, r0
#endif

#ifndef __SOFTFP__
# if __ARM_ARCH__ >= 7
	vstmia	ip!, {d0-d15}
#   ifdef __ARM_NEON__
	vstmia	ip!, {d16-d31}
#   endif
# else
	sfm	f4, 4, [ip]!
# endif
#endif

#ifdef __IWMMXT__
#  warning "setjmp will not save iwmmxt coprocessor registers"
#endif

#ifndef DIET_HAVE_COPROC_REGS
	stmia	r0, {r4-r11, sp, lr}
#else
	stmia	ip!, {r4-r11, sp, lr}
#endif

	b	__sigjmp_save

