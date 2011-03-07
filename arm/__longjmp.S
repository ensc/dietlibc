.text
.global __longjmp
.type __longjmp,function
__longjmp:
	mov	ip, r0
	movs	r0, r1
	moveq	r0, #1
#ifndef	__SOFTFP__
	lfm	f4, 4, [ip, #10*4]
#endif
#ifdef __IWMMXT__
#  warning "sigjmp will not restore iwmmxt coprocessor registers"
#endif
	ldmia   ip, {r4-r11, sp, pc}
