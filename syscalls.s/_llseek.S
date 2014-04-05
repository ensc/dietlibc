#include "syscalls.h"

#ifdef __NR__llseek
.global llseek
.type	llseek,STT_FUNC
llseek:
syscall(_llseek,_llseek)
#endif
