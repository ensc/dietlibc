#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <sys/cdefs.h>

#include <linux/types.h>
#include <linux/signal.h>

typedef void (*sighandler_t)(int);

int sigemptyset(sigset_t *set) __THROW;
int sigaddset(sigset_t *set, int signum) __THROW;
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset) __THROW;
sighandler_t signal(int signum, sighandler_t action);
int raise (int sig) __THROW;

#endif
