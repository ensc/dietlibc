#include "syscalls.h"

_syscall3(int,syslog,int, type, char *, buf, int, len);
