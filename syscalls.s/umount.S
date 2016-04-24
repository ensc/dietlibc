#include "syscalls.h"

#ifdef __NR_umount
#if defined(__NR_umount_without_flags)
syscall(umount_without_flags,umount)
#elif !defined(__NR_umount_with_flags) || (__NR_umount != __NR_umount_with_flags)
syscall(umount,umount)
#endif
#endif
