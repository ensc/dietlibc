#define zero 0
#define ra 31
#define sp 29
#define a0 4
#define a1 5
#define a2 6

.text
.global __start
.type __start,@function
__start:
	.set noreorder
	bltzal $0,0f
	nop
0:	.cpload	$31
	.set reorder
	move	$ra, $zero	/* prime stack frame */
	lw	$a0, 0($sp)	/* load argc */
	addu	$a1, $sp, 4	/* load argv.  huh?  should be 4, right? */
	addu	$a2, $a0, 1	/* load envp */
	sll	$a2, $a2, 2
	add	$a2, $a2, $a1
	sw      $a2,environ
	jal	main
	move	$4,$2
	jal	exit

.global __main
.type __main,@function
__main:
	j	$31
