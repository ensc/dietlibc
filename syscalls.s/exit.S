#include "syscalls.h"

.text
.weak exit
exit:
syscall(exit,_exit)
