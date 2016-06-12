#include "syscalls.h"
#include "ppc64-features.h"

		.text
diet_proto(umount)
FN_NAME_DOTIFY(umount):
#if defined(_CALL_ELF) && _CALL_ELF == 2
	addis	2,12,.TOC.-umount@ha
	addi	2,2,.TOC.-umount@l
.localentry	umount,.-umount
#endif
		li 0, __NR_umount2
		li 4, 0
		b __unified_syscall
