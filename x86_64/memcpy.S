.text
.global memcpy
#ifdef __PIE__
.hidden memcpy
#endif
.type memcpy,@function
memcpy:	/* rdi=dest, rsi=src, rdx=len */
  .cfi_startproc
  mov %rdi,%rax
  mov %rdx,%rcx
  cmp $16,%rcx
  jb .Lrepmovsb
  /* sse2 version for larger strings; len >= 16 */
  add %rdx,%rsi	/* rsi = src+len */
  add %rdx,%rdi	/* rdi = dest+len */
  mov %rdx,%rcx
  neg %rcx	/* rcx = -len */
  /* x86 addressing modes allow calculating rsi+rcx*16 inline.
     we are set up now so we only need to increment one register in the
     loop, instead of adding 16 to two registers and substracting 16
     fron third one. */
  and $15,%rdx
  /* We want to copy 16 bytes blocks at a time, but len is probably not
     a multiple of 16. So after the first block, we don't go forward 16,
     we go forward len%16 */
  jz 1f		/* skip if len%16 == 0 */
  movups (%rsi,%rcx),%xmm0
  movups %xmm0,(%rdi,%rcx)
  add %rdx,%rcx
1:
  and $-16,%rcx
2:
  movups (%rsi,%rcx),%xmm0
  movups %xmm0,(%rdi,%rcx)
  add $16,%rcx
  jnz 2b
3:
  ret
.Lrepmovsb:
  /* traditional version for short strings */
  rep movsb
  ret
  .cfi_endproc

.Lhere:
.size memcpy,.Lhere-memcpy
	.section	.note.GNU-stack,"",@progbits
