#ifndef _SYS_FILE_H
#define _SYS_FILE_H

#include <sys/cdefs.h>
#include <fcntl.h>

extern int fcntl(int fd, int cmd, ...) __THROW;
extern int flock(int fd, int operation) __THROW;

#endif	/* _SYS_FILE_H */
