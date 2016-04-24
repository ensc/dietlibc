#include "syscalls.h"

#if defined(__NR_umount_with_flags)
syscall(umount_with_flags,umount2)
#elif defined(__NR_umount2)
syscall(umount2,umount2)
#endif
