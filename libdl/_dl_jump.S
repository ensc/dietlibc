.text
.global _dl_jump
_dl_jump:
#ifdef __DYN_LIB
	call	do_rel@PLT
#else
	call	do_rel
#endif
	pop	%edx
	pop	%edx
	jmp	*%eax

