.text
.global _dl_jump
_dl_jump:
	call	do_rel
	pop	%edx
	pop	%edx
	jmp	*%eax

