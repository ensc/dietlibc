#include <errno.h>
#include "syscalls.h"
#include "mips-asm.h"

.text
.align 2
.global __clone
.ent	__clone
.type __clone,@function
__clone:
	.set noreorder
	.cpload $25
	.set reorder
#if _MIPS_SIM == _ABI64
	dsubu	$29,$29,64
#else
	subu	$29,$29,32
#endif
	.cprestore 16

	li	$2, EINVAL
	beqz	$4, .Lerror
	beqz	$5, .Lerror

#if _MIPS_SIM == _ABI64
	dsubu	$5, $5, 64	/* resterve some space */
	sd	$4, 0($5)	/* save function pointer */
	sd	$7, 8($5)	/* save arg pointer */
#else
	subu	$5, $5, 32	/* resterve some space */
	sw	$4, 0($5)	/* save function pointer */
	sw	$7, 4($5)	/* save arg pointer */
#endif

	move	$4, $6
	li	$2, __NR_clone
	syscall

	bnez	$7, .Lerror
	beqz	$2, .Lstart

#if _MIPS_SIM == _ABI64
	daddu	$29,$29,64	/* free stack */
#else
	addu	$29,$29,32	/* free stack */
#endif
	jr	$31

.Lerror:
	PTR_LA	$25, error_unified_syscall
	jr	$25

.Lstart:
#if _MIPS_SIM == _ABI64
	ld	$28,32($29)
	ld	$25, 0($29)
	ld	$4,  8($29)
#else
	lw	$28,16($29)
	lw	$25, 0($29)
	lw	$4,  4($29)
#endif
	jalr	$25
	move	$4, $2
	jal	_exit

.end	__clone
