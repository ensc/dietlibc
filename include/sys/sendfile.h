#ifndef _SYS_SENDFILE_H
#define _SYS_SENDFILE_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/stat.h>

extern ssize_t sendfile (int out_fd, int in_fd, off_t* offset,
			 size_t count) __THROW;

#ifndef __NO_STAT64
extern ssize_t sendfile64 (int out_fd, int in_fd, loff_t* offset,
			   size_t count) __THROW;

#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
#define sendfile(outfd,infd,offset,count) sendfile64(outfd,infd,offset,count)
#endif
#endif

#endif	/* sys/sendfile.h */
