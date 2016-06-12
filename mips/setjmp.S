.text
.align 2
setjmp:
.global __setjmp
.ent	__setjmp
__setjmp:
	li	$5, 0		/* don't save sigmask */
.end	__setjmp
.global __sigsetjmp
.ent	__sigsetjmp
__sigsetjmp:
#if _MIPS_SIM == _ABI64
	/* where to jump later */
	dla	$25, __sigjmp_save
	/* callee save floating point save registers (double) */
	s.d	$f24, 104($4)	/* fs0 */
	s.d	$f25, 112($4)	/* fs1 */
	s.d	$f26, 120($4)	/* fs2 */
	s.d	$f27, 128($4)	/* fs3 */
	s.d	$f28, 136($4)	/* fs4 */
	s.d	$f29, 144($4)	/* fs5 */
	s.d	$f30, 152($4)	/* fs6 */
	s.d	$f31, 160($4)	/* fs7 */
	/* save coprocessor status register */
	cfc1	 $2, $31
	/* callee save register */
	sd	$gp, 88($4)
	sd	 $2, 96($4)
	sd	$31,  0($4)	/* ra */
	sd	$29,  8($4)	/* sp */
	sd	$16, 16($4)	/* s0 */
	sd	$17, 24($4)	/* s1 */
	sd	$18, 32($4)	/* s2 */
	sd	$19, 40($4)	/* s3 */
	sd	$20, 48($4)	/* s4 */
	sd	$21, 56($4)	/* s5 */
	sd	$22, 64($4)	/* s6 */
	sd	$23, 72($4)	/* s7 */
	sd	$30, 80($4)	/* fp */
#else
	/* where to jump later */
	la	$25, __sigjmp_save
	/* callee save floating point save registers (double) */
	s.d	$f20, 56($4)	/* fs0 */
	s.d	$f22, 64($4)	/* fs1 */
	s.d	$f24, 72($4)	/* fs2 */
	s.d	$f26, 80($4)	/* fs3 */
	s.d	$f28, 88($4)	/* fs4 */
	s.d	$f30, 96($4)	/* fs6 */
	/* save coprocessor status register */
	cfc1	 $2, $31
	/* callee save register */
	sw	$gp, 44($4)
	sw	 $2, 48($4)
	sw	$31,  0($4)	/* ra */
	sw	$29,  4($4)	/* sp */
	sw	$16,  8($4)	/* s0 */
	sw	$17, 12($4)	/* s1 */
	sw	$18, 16($4)	/* s2 */
	sw	$19, 20($4)	/* s3 */
	sw	$20, 24($4)	/* s4 */
	sw	$21, 28($4)	/* s5 */
	sw	$22, 32($4)	/* s6 */
	sw	$23, 36($4)	/* s7 */
	sw	$30, 40($4)	/* fp */
#endif
	j	$25
.end	__sigsetjmp
.weak	setjmp
