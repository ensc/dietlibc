#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <sys/cdefs.h>

#include <linux/types.h>
#include <linux/signal.h>

int sigemptyset(sigset_t *set) __THROW;
int sigaddset(sigset_t *set, int signum) __THROW;
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset) __THROW;
int raise (int sig) __THROW;

#endif
