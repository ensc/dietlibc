#ifndef _SYS_POLL_H
#define _SYS_POLL_H

#include <sys/cdefs.h>

#include <linux/poll.h>

extern int poll(struct pollfd *ufds, unsigned int nfds, int timeout) __THROW;


#endif	/* _SYS_POLL_H */
