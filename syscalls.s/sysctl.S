#include "syscalls.h"

#ifdef __NR_sysctl
syscall(sysctl,_sysctl)
#endif
