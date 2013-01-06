#include "syscalls.h"

#ifdef __NR_fadvise64_64
syscall(fadvise64_64, __s390_fadvise64_64);
#endif
