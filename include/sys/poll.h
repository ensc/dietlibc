#ifndef _SYS_POLL_H
#define _SYS_POLL_H

#include <sys/cdefs.h>

#define POLLIN		0x0001
#define POLLPRI		0x0002
#define POLLOUT		0x0004
#define POLLERR		0x0008
#define POLLHUP		0x0010
#define POLLNVAL	0x0020
#define POLLRDNORM	0x0040
#define POLLRDBAND	0x0080
#define POLLWRBAND	0x0200
#define POLLMSG		0x0400

#if defined(__sparc__) || defined (__mips__)
#define POLLWRNORM	POLLOUT
#else
#define POLLWRNORM	0x0100
#endif

struct pollfd {
  int fd;
  short events;
  short revents;
};

extern int poll(struct pollfd *ufds, unsigned int nfds, int timeout) __THROW;


#endif	/* _SYS_POLL_H */
