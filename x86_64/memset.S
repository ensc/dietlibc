.text
.global memset
#ifdef __PIE__
.hidden memset
#endif
.type memset,@function
memset:	/* rdi=ptr, esi=ch, rdx=len */
  movzbl %sil,%eax
  movabs $0x101010101010101,%rsi
  imul %rsi,%rax	/* rax = ch x 8 */
  mov %rdi,%rsi
  mov %rdx,%rcx
  shr $3,%rcx
  rep stosq
  mov %rdx,%rcx
  and $7,%rcx
  rep stosb
  mov %rsi,%rax
  ret

.Lhere:
.size memset,.Lhere-memset
	.section	.note.GNU-stack,"",@progbits
