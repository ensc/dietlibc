#include "syscalls.h"

.weak __libc_connect
syscall(connect,connect)
