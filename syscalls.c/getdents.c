#include "syscalls.h"

_syscall3(int, getdents, unsigned int, fd, void *, dirp, unsigned int, count);

