.text
.global _dl_jump
_dl_jump:

#ifdef __i386__
.type	_dl_jump,@function

#ifdef __DYN_LIB
	pushl	%ebx
	movl	%esp,%ebx
	pushl	8(%ebx)
	pushl	4(%ebx)
	call	1f
1:	popl	%ebx
	addl	$_GLOBAL_OFFSET_TABLE_+[.-1b],%ebx
	call	do_rel@PLT
	addl	$8,%esp
	popl	%ebx
#else
	call	do_rel
#endif
	addl	$8,%esp
	jmp	*%eax

#endif

#ifdef __arm__
.type	_dl_jump,function

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
