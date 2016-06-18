#include <poll.h>
#include <signal.h>
#include "syscalls.h"

#ifdef __NR_ppoll
extern int __ppoll(struct pollfd* fds, nfds_t nfds,
		   const struct timespec *timeout,
		   const sigset_t* sigmask, size_t sigsetsize);

int ppoll(struct pollfd* fds, nfds_t nfds, const struct timespec *timeout,
		const sigset_t* sigmask)
{
	return __ppoll(fds, nfds, timeout, sigmask, _NSIG/8);
}
#endif
