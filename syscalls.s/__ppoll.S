#include "syscalls.h"

#ifdef __NR_ppoll
syscall(ppoll,__ppoll)
#endif
