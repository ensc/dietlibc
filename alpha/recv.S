#include "syscalls.h"

.weak __libc_recv
syscall(recv,recv)
