.text
.global memcpy
.type memcpy,@function
memcpy:	/* rdi=dest, esi=src, rdx=len */
  mov %rdi,%rax
  mov %rdx,%rcx
  shr $3,%rcx
  rep movsq
  mov %rdx,%rcx
  and $7,%rcx
  rep movsb
  ret

.Lhere:
.size memcpy,.Lhere-memcpy
	.section	.note.GNU-stack,"",@progbits
