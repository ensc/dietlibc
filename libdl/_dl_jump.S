.text
.global _dl_jump
_dl_jump:

#ifdef __i386__

#ifdef __DYN_LIB
	call	do_rel@PLT
#else
	call	do_rel
#endif
	pop	%edx
	pop	%edx
	jmp	*%eax

#endif

#ifdef __arm__
	stmdb	sp!, {r0, r1, r2, r3, lr, pc}
#ifdef __DYN_LIB
	bl	do_rel(PLT)
#else
	bl	do_rel
#endif
	str	r0, [sp, #20]		/* overwrite pc on stack */
	ldmia	sp!, {r0, r1, r2, r3, lr, pc}

#endif
