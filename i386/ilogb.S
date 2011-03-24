#
#   int  ilogb ( double x );
#
#   returns  (int) log2 (fabs(x))  for x!=0
#   returns  MIN_INT               for x==0
#

.text
.global ilogb
.type   ilogb,@function

ilogb:
	movl	8(%esp),%eax
	addl	%eax,%eax
	jz	.Lzero
	shrl	$21,%eax
	subl	$1023,%eax
	ret
	
.Lzero:
	stc
	rcrl	%eax
	ret	

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
