.text
.align 2
.global __longjmp
.ent	__longjmp
__longjmp:
#if _MIPS_SIM == _ABI64
	/* load coprocessor status */
	lw	 $2, 96($4)
	/* restore callee save floating point registers (double) */
	l.d	$f24, 104($4)	/* fs0 */
	l.d	$f25, 112($4)	/* fs1 */
	l.d	$f26, 120($4)	/* fs2 */
	l.d	$f27, 128($4)	/* fs3 */
	l.d	$f28, 136($4)	/* fs4 */
	l.d	$f29, 144($4)	/* fs5 */
	l.d	$f30, 152($4)	/* fs6 */
	l.d	$f31, 160($4)	/* fs7 */
	/* restore coprocessor status register */
	ctc1	 $2, $31
	/* restore callee save integer register */
	ld	$25,  0($4)	/* ra */
	ld	$29,  8($4)	/* sp */
	ld	$16, 16($4)	/* s0 */
	ld	$17, 24($4)	/* s1 */
	ld	$18, 32($4)	/* s2 */
	ld	$19, 40($4)	/* s3 */
	ld	$20, 48($4)	/* s4 */
	ld	$21, 56($4)	/* s5 */
	ld	$22, 64($4)	/* s6 */
	ld	$23, 72($4)	/* s7 */
	ld	$30, 80($4)	/* fp/s8 */
	ld	$gp, 88($4)	/* gp */
#else
	/* load coprocessor status */
	lw	 $2, 48($4)
	/* restore callee save floating point registers (double) */
	l.d	$f20, 56($4)	/* fs0 */
	l.d	$f22, 64($4)	/* fs1 */
	l.d	$f24, 72($4)	/* fs2 */
	l.d	$f26, 80($4)	/* fs3 */
	l.d	$f28, 88($4)	/* fs4 */
	l.d	$f30, 96($4)	/* fs6 */
	/* restore coprocessor status register */
	ctc1	 $2, $31
	/* restore callee save integer register */
	lw	$25,  0($4)	/* ra */
	lw	$29,  4($4)	/* sp */
	lw	$16,  8($4)	/* s0 */
	lw	$17, 12($4)	/* s1 */
	lw	$18, 16($4)	/* s2 */
	lw	$19, 20($4)	/* s3 */
	lw	$20, 24($4)	/* s4 */
	lw	$21, 28($4)	/* s5 */
	lw	$22, 32($4)	/* s6 */
	lw	$23, 36($4)	/* s7 */
	lw	$30, 40($4)	/* fp/s8 */
	lw	$gp, 44($4)	/* gp */
#endif
	/* never return 0 */
	move	 $2, $5
	bne	 $5, $0,1f
	li	 $2, 1
1:	j	$25
.end	__longjmp
