#ifndef _SYS_EPOLL_H
#define _SYS_EPOLL_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/poll.h>

/* Valid opcodes ( "op" parameter ) to issue to epoll_ctl() */
#define EPOLL_CTL_ADD 1	/* Add a file decriptor to the interface */
#define EPOLL_CTL_DEL 2	/* Remove a file decriptor from the interface */
#define EPOLL_CTL_MOD 3	/* Change file decriptor epoll_event structure */

int epoll_create(int size) __THROW;
int epoll_ctl(int epfd, int op, int fd, unsigned int events) __THROW;
int epoll_wait(int epfd, struct pollfd *events, int maxevents,
	       int timeout) __THROW;

#endif
