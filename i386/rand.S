.data
.align 4
.type	seed,@object
.size	seed,4
seed:
	.long 1

.text
.global rand
.type	rand,@function
.global random
.type	random,@function

rand:
random:
#ifdef __DYN_LIB
	pushl	%ebx
	call	1f
1:	popl	%ebx
	addl	$_GLOBAL_OFFSET_TABLE_+[.-1b],%ebx
	imull	$1103515245,seed@GOTOFF(%ebx),%eax
#else
	imull	$1103515245,seed,%eax
#endif
	addl	$12345,%eax
#ifdef __DYN_LIB
.Lstore:movl	%eax,seed@GOTOFF(%ebx)
#else
.Lstore:movl	%eax,seed
#endif
	andl	$0x7FFFFFFF,%eax
#ifdef __DYN_LIB
	popl	%ebx
#endif
	ret

.global srand
.type   srand,@function
.global srandom
.type   srandom,@function

srand:
srandom:
#ifdef __DYN_LIB
	pushl	%ebx
	call	1f
1:	popl	%ebx
	addl	$_GLOBAL_OFFSET_TABLE_+[.-1b],%ebx
#endif
	movl	4(%esp),%eax
	jmp	.Lstore
