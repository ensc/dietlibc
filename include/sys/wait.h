#ifndef _SYS_WAIT_H
#define _SYS_WAIT_H

#include <sys/cdefs.h>
#include <sys/resource.h>
#include <linux/wait.h>

/* If WIFEXITED(STATUS), the low-order 8 bits of the status.  */
#define __WEXITSTATUS(status)	(((status) & 0xff00) >> 8)
#define WEXITSTATUS __WEXITSTATUS

/* If WIFSIGNALED(STATUS), the terminating signal.  */
#define __WTERMSIG(status)	((status) & 0x7f)
#define WTERMSIG __WTERMSIG

/* If WIFSTOPPED(STATUS), the signal that stopped the child.  */
#define __WSTOPSIG(status)	__WEXITSTATUS(status)
#define WSTOPSIG __WSTOPSIG

/* Nonzero if STATUS indicates normal termination.  */
#define WIFEXITED(status)	(__WTERMSIG(status) == 0)

/* Nonzero if STATUS indicates termination by a signal.  */
#define WIFSIGNALED(status)	(!WIFSTOPPED(status) && !WIFEXITED(status))

/* Nonzero if STATUS indicates the child is stopped.  */
#define WIFSTOPPED(status)	(((status) & 0xff) == 0x7f)


pid_t wait(int *status) __THROW;
pid_t waitpid(pid_t pid, int *status, int options) __THROW;

pid_t wait3(int *status, int options, struct rusage *rusage) __THROW;

pid_t wait4(pid_t pid, int *status, int options, struct rusage *rusage) __THROW;

#endif
