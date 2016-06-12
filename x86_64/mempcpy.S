.text
.global mempcpy
#ifdef __PIE__
.hidden mempcpy
#endif
.type mempcpy,@function
mempcpy:	/* rdi=dest, rsi=src, rdx=len */
  mov %rdx,%rcx
1:
  test $16,%ecx
  jnc 1f
  movups (%rsi),%xmm0
  movups %xmm0,(%rdi)
  add $16,%rsi
  add $16,%rdi
  sub $16,%rcx
  jmp 1b
1:
  shr $3,%rcx
  rep movsq
  mov %rdx,%rcx
  and $7,%rcx
  rep movsb
  mov %rdi,%rax
  ret

.Lhere:
.size mempcpy,.Lhere-mempcpy
	.section	.note.GNU-stack,"",@progbits
