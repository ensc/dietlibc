#include "syscalls.h"

.weak __libc_send
syscall(send,send)
