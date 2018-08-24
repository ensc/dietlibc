#include <dietfeatures.h>
#include "syscalls.h"

	.text
	.align	2
	.weak exit
exit:
	.global _Exit
	.ent _Exit
_Exit:
	.global _exit
	.ent _exit
_exit:
	li	$2,__NR_exit
	syscall
	.end _exit
	.globl	__unified_syscall
	.ent	__unified_syscall
__unified_syscall:
	bne	$7, $0, .Lerror

/* here we go and "reuse" the return for weak-void functions */
#include "dietuglyweaks.h"

	jr	$31
	.end	__unified_syscall

	.global error_unified_syscall
	.ent	error_unified_syscall
error_unified_syscall:
.Lerror:
#ifdef WANT_THREAD_SAFE
#if _MIPS_SIM == _ABI64
	daddiu	$sp, $sp, -48

	sd	$31, 0($sp)
	sd	$2,  8($sp)

	dla	$25, __errno_location
	jalr	$25

	ld	$8,  8($sp)
	ld	$31, 0($sp)
	sd	$8,  0($2)
	li	$2,  -1
	daddiu	$sp, $sp, 48
#else
	addiu	$sp, $sp, -24

	sw	$31, 0($sp)
	sw	$2,  4($sp)

	la	$25, __errno_location
	jalr	$25

	lw	$8,  4($sp)
	lw	$31, 0($sp)
	sw	$8,  0($2)
	li	$2,  -1
	addiu	$sp, $sp, 24
#endif
	jr	$31
#else
	PTR_SW	$2,  errno
	li	$2,  -1
	jr	$31
#endif
	.end	error_unified_syscall
