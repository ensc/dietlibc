#ifndef _FCNTL_H
#define _FCNTL_H

#include <sys/cdefs.h>

#include <sys/types.h>
#include <unistd.h>

#include <asm/fcntl.h>

extern int fcntl (int __fd, int __cmd, ...) __THROW;
extern int open (const char *__file, int __oflag, ...) __THROW;
extern int open64 (const char *__file, int __oflag, ...) __THROW;
extern int creat (const char *__file, mode_t __mode) __THROW;
extern int creat64 (const char *__file, mode_t __mode) __THROW;
extern int lockf (int __fd, int __cmd, off_t __len) __THROW;
extern int lockf64 (int __fd, int __cmd, off64_t __len) __THROW;

#define F_ULOCK 0	/* Unlock a previously locked region.  */
#define F_LOCK  1	/* Lock a region for exclusive use.  */
#define F_TLOCK 2	/* Test and lock a region for exclusive use.  */
#define F_TEST  3	/* Test a region for other processes locks.  */

#endif
