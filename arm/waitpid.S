	.text
	.align	2
	.global	waitpid
	.type	waitpid, %function
waitpid:
	mov	r3, #0
	b	wait4
	.size	waitpid, .-waitpid
