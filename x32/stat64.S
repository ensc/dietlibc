#include "dietfeatures.h"
#include "x86_64/syscalls.h"

.text
.global stat64
stat64:
	xor %eax,%eax
	mov $__NR_stat,%al
#ifdef __PIC__
	jmp __unified_syscall_lp64@PLT
#else
	jmp __unified_syscall_lp64
#endif

