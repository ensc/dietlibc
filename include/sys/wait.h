#ifndef _SYS_WAIT_H
#define _SYS_WAIT_H

#include <sys/cdefs.h>
#include <sys/resource.h>
#include <linux/wait.h>

pid_t wait(int *status) __THROW;
pid_t waitpid(pid_t pid, int *status, int options) __THROW;

pid_t wait3(int *status, int options, struct rusage *rusage) __THROW;

pid_t wait4(pid_t pid, int *status, int options, struct rusage *rusage) __THROW;

#endif
