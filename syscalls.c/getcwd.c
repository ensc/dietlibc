#include "syscalls.h"

/* this syscall exists since Linux 2.1.92 */

_syscall2(int,getcwd,char*,buf,unsigned long,size)
