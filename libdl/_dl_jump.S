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
	stmdb	sp!, {r0, r1, r2, r3}
	sub	r1, ip, lr
	sub	r1, r1, #4
	add	r1, r1, r1		/* dyntab entry */
	ldr	r0, [lr, #-4]		/* dynlib handle */
#ifdef __DYN_LIB
	bl	do_rel(PLT)
#else
	bl	do_rel
#endif
	mov	r12, r0
	ldmia	sp!, {r0, r1, r2, r3, lr}
	mov	pc, r12

#endif
