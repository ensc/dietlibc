#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <sys/cdefs.h>

#define __WANT_POSIX1B_SIGNALS__

#include <linux/types.h>
#include <linux/signal.h>

typedef void (*sighandler_t)(int);

int sigemptyset(sigset_t *set) __THROW;
int sigfillset(sigset_t *set) __THROW;
int sigaddset(sigset_t *set, int signum) __THROW;
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset) __THROW;
sighandler_t signal(int signum, sighandler_t action);
int raise (int sig) __THROW;
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) __THROW;
int sigaltstack(const struct sigaltstack *newstack, struct sigaltstack *oldstack) __THROW;

#ifndef SIGCLD
#define SIGCLD SIGCHLD
#endif

extern const char *const sys_siglist[];

typedef __sighandler_t sig_t;

#ifndef NSIG
#define NSIG _NSIG
#endif

#endif
