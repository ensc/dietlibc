.text
.align 2
.global __longjmp
.ent	__longjmp
__longjmp:
	/* restore FP callee-save regiser */
	l.d	$f20, 56($4)
	l.d	$f22, 64($4)
	l.d	$f24, 72($4)
	l.d	$f26, 80($4)
	l.d	$f28, 88($4)
	l.d	$f30, 96($4)
	/* restore Coprocessor Status Register */
	lw	 $2, 48($4)
	lw	$gp, 44($4)
	ctc1	 $2, $31
	/* restore callee-save register */
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
	lw	$30, 40($4)	/* fp */

	/* never return 0 */
	move	$2, $5
	bne	$5, $0, $L3
	li	$2, 1
$L3:
	jr	$25
.end	__longjmp
