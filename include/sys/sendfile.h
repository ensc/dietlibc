#ifndef _SYS_SENDFILE_H
#define _SYS_SENDFILE_H

#include <sys/cdefs.h>
#include <sys/types.h>

extern ssize_t sendfile (int __out_fd, int __in_fd, off_t *offset,
			 size_t __count) __THROW;

#endif	/* sys/sendfile.h */
