#include "dietfeatures.h"
#include "syscalls.h"

#ifdef __NR__llseek
syscall(_llseek,llseek)
#endif
