#include <errno.h>
#include "syscalls.h"

.text
.align 2
.global __clone
.ent	__clone
.type __clone,@function
__clone:
	.set noreorder
	bltzal $0,0f
	nop
0:	.cpload	$31
	.set reorder

	li	$0, EINVAL
	beqz	$4, .Lerror
	beqz	$5, .Lerror

	subu	$5, 32		/* resterve some space */
	sw	$4, 0($5)	/* save function pointer */
	sw	$7, 4($5)	/* save arg pointer */

	move	$4, $6
	li	$2, __NR_clone
	syscall

	bnez	$7, .Lerror
	beqz	$2, .Lstart

	jr	$31
	.cprestore
.end	__clone

.Lerror:
	la	$25, error_unified_syscall
	jr	$25

.Lstart:
	lw	$25, 0($29)
	lw	$4,  4($29)
	jalr	$25
	move	$4, $2
	jal	_exit
