#include "parisc/syscalls.h"

.text
.type __dietlibc_truncate64,@function
.globl __dietlibc_truncate64
.export __dietlibc_truncate64
__dietlibc_truncate64:
#if !defined(__LP64__)
	copy %arg3,%arg1
#endif
	b __unified_syscall
	ldi __NR_truncate64, %r20
