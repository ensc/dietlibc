#include "syscalls.h"

#ifdef __NR_signalfd
syscall(signalfd4,__signalfd4)
#endif
