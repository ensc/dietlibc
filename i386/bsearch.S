.text
.global bsearch 
.type	 bsearch,@function
bsearch:
	push %ebx
	push %esi
	push %edi
	
	movl 20(%esp), %esi			# base
	movl 24(%esp), %ecx			# nnemb
	movl 28(%esp), %edx			# size
		
	jecxz .LreturnNULL

	leal -2(%ecx), %eax
	imull %edx, %eax
	leal (%esi, %eax), %edi

.LLoop:
	movl %ecx, %ebx
	shrl $1, %ebx
	imull %edx, %ebx
	addl %esi, %ebx

	pushl %ecx
	pushl %edx
	
	pushl %ebx
	pushl 28(%esp)				# key 
	call *48(%esp)
	addl $8, %esp

	popl %edx
	popl %ecx

	testl %eax, %eax
	js .LlowerZero
;;		testl %eax, %eax
		jz .Lreturn
		leal (%ebx, %edx), %esi
		movl %ecx, %eax
		shrl $1, %eax
		subl %eax, %ecx
		jmp .LtestDone
.LlowerZero:
		movl %ebx, %edi
		subl %edx, %edi
		shrl $1, %ecx
.LtestDone:

	cmp %edi, %esi
	jbe .LLoop
	
.LreturnNULL:
	xorl %ebx, %ebx

.Lreturn:
	movl %ebx, %eax 

	pop %edi
	pop %esi
	pop %ebx
	ret
.Lende:
.size	bsearch,.Lende-bsearch
