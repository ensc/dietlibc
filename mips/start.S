#include "dietfeatures.h"
#include "mips-asm.h"

#define zero 0
#define ra 31
#define sp 29
#define a0 4
#define a1 5
#define a2 6
#define gp 28

.text
.global __start
.type __start,@function
__start:
#ifdef __pic__
/* initialize gp pointer */
	.set noreorder
	bltzal $0,0f
	nop
0:	.cpload	$31
	.set reorder
#else
	/* _gp is provided by the linker and points into the middle of the
	 * "small data" section (.sdata and .sbss) allocated by the
	 * -G 8 compiler option */
	PTR_LA  $gp, _gp
#endif
	move	$ra, $zero	/* prime stack frame */
	lw	$a0, 0($sp)	/* load argc */
#if _MIPS_SIM == _ABI64
	/* The same lack of clarity w.r.t. the MIPS calling conventions
	 * apply here as with MIPS32. (See below.) */
	daddu	$a1, $sp, 8	/* load argv */

	li	$t0, -8
	and	$sp, $sp, $t0
	dsubu	$sp, 48		/* make room for 4 arguments, RA + pad */
	sd	$ra, 40($sp)	/* close stack frame */
#else
	addu	$a1, $sp, 4	/* load argv.  huh?  should be 4, right? */

	and	$sp, 0xfffffff8	/* align stack to 8 bytes */
	subu	$sp, 24		/* make room for 4 arguments, RA + pad */
	/* I don't understand the MIPS calling convention.  Why do you
	   need to make room on the stack for arguments you pass in
	   registers?  Anyway, if we don't do this, the arguments are
	   garbled. */
	sw	$ra, 20($sp)	/* close stack frame */
#endif

	addu	$a2, $a0, 1	/* load envp */
#if _MIPS_SIM == _ABI64
	dsll	$a2, $a2, 3
#else
	sll	$a2, $a2, 2
#endif
#ifdef WANT_DYNAMIC
/* FIXME: dl_init parameter ??? */
	PTR_LA	$25, _dyn_start
#else
	PTR_LA	$25, CALL_IN_STARTCODE
#endif
	PTR_ADD	$a2, $a2, $a1
	PTR_SW	$a2, environ
#ifdef WANT_ELFINFO
1:	addu	$a2, $a2, 4	/* increment envp */
	lw	$4, -4($a2)     /* load envp[-1]; TODO: is $4 a proper
				   temporary register? */
	bne	$4, $0, 1b	/* ... until envp[-1]==NULL
				   TODO: use proper 'bnz' operation */
	sw	$a2, __elfinfo
#endif
	jalr	$25
	PTR_LA	$25, exit
	move	$4,$2
	jalr	$25

.global __main
.type __main,@function
__main:
	j	$31
