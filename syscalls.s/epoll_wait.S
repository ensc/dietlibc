#include "syscalls.h"

#ifdef __NR_sys_epoll_wait
syscall(sys_epoll_wait,epoll_wait)
#endif
