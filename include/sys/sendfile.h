#ifndef _SYS_SENDFILE_H
#define _SYS_SENDFILE_H

#include <sys/cdefs.h>
#include <sys/types.h>

extern ssize_t sendfile (int out_fd, int in_fd, off_t* offset,
			 size_t count) __THROW;

extern ssize_t sendfile64 (int out_fd, int in_fd, loff_t* offset,
			   size_t count) __THROW;

#endif	/* sys/sendfile.h */
