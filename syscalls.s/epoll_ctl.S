#include "syscalls.h"

#ifdef __NR_sys_epoll_ctl
syscall(sys_epoll_ctl,epoll_ctl)
#endif
