#ifndef _SYS_EPOLL_H
#define _SYS_EPOLL_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/poll.h>

/* Valid opcodes ( "op" parameter ) to issue to epoll_ctl() */
#define EPOLL_CTL_ADD 1	/* Add a file decriptor to the interface */
#define EPOLL_CTL_DEL 2	/* Remove a file decriptor from the interface */
#define EPOLL_CTL_MOD 3	/* Change file decriptor epoll_event structure */

typedef union epoll_data {
  void *ptr;
  int fd;
  __uint32_t u32;
  __uint64_t u64;
} epoll_data_t;

struct epoll_event {
  uint32_t events;
  epoll_data_t data;
};

int epoll_create(int size) __THROW;
int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event) __THROW;
int epoll_wait(int epfd, struct epoll_event *events, int maxevents,
	       int timeout) __THROW;

#endif
