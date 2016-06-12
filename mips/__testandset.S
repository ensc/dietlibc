#include "mips-asm.h"

.text
.align 2
.global __testandset
.ent	__testandset
.type	__testandset,@function
__testandset:
1:
#if _MIPS_SIM != _ABI64
	.set	mips2
#endif
	ll	$2, 0($4)
	.set	noreorder
	bnez	$2, 1f
	li	$8, 1
	.set	reorder
	sc	$8, 0($4)
	beqz	$8, 1b
1:
#if _MIPS_SIM != _ABI64
	.set	mips0
#endif
	jr	$31
.end	__testandset
