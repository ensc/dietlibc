#include "start.h"

.text
.global __clone
.type __clone,@function
__clone:
	.set noreorder
	bltzal $0,0f
	nop
0:	.cpload	$31
	.set reorder

	li	$0, EINVAL
	beqz	$4, .Lerror
	begz	$5, .Lerror

	subu	$5, 32		/* resterve some space */
	sw	$4, 0($5)	/* save function pointer */
	sw	$7, 4($5)	/* save arg pointer */

	move	$4, $6
	li	$2, __NR_clone
	syscall

	bnez	$7, error
	beqz	$2, .Lstart

	ret

.Lerror:
	j	error_unified_syscall

.Lstart:
	lw	$25, 0($29)
	lw	$4,  4($29)
	jalr	$25
	move	$4, $2
	jal	_exit
