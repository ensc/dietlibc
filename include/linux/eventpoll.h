#ifndef _LINUX_EVENTPOLL_H
#define _LINUX_EVENTPOLL_H

#include <sys/cdefs.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/shm.h>
#include <sys/epoll.h>

__BEGIN_DECLS

#define POLLFD_X_PAGE	(PAGE_SIZE / sizeof(struct pollfd))
#define EP_FDS_PAGES(n)	(((n) + POLLFD_X_PAGE - 1) / POLLFD_X_PAGE)
#define EP_MAP_SIZE(n)	(EP_FDS_PAGES(n) * PAGE_SIZE * 2)

#ifdef __x86_64__
#define EPOLL_PACKED __attribute__((packed))
#else
#define EPOLL_PACKED
#endif

struct evpoll {
  int ep_timeout;
  unsigned long ep_resoff;
} EPOLL_PACKED;

__END_DECLS

#endif
