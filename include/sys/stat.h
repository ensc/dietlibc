#ifndef _SYS_STAT_H
#define _SYS_STAT_H

#include <sys/cdefs.h>
#include <sys/types.h>

#include <linux/stat.h>
#include <asm/stat.h>

#ifdef __alpha__
#define __NO_STAT64
#endif

#define stat(file,buf) __dietstat(file,buf)
#define fstat(file,buf) __dietfstat(file,buf)
#define lstat(file,buf) __dietlstat(file,buf)

#define stat64(file,buf) __dietstat64(file,buf)
#define fstat64(file,buf) __dietfstat64(file,buf)
#define lstat64(file,buf) __dietlstat64(file,buf)

#define major(dev) (((dev)>>8) & 0xff)
#define minor(dev) ((dev) & 0xff)
#define makedev(major, minor) ((((unsigned int) (major)) << 8) | ((unsigned int) (minor)))

extern int __dietstat (const char *__file, struct stat *__buf) __THROW;
extern int __dietfstat (int __fd, struct stat *__buf) __THROW;
extern int __dietlstat (const char *__file, struct stat *__buf) __THROW;

#ifndef __NO_STAT64
extern int __dietstat64 (const char *__file, struct stat64 *__buf) __THROW;
extern int __dietfstat64 (int __fd, struct stat64 *__buf) __THROW;
extern int __dietlstat64 (const char *__file, struct stat64 *__buf) __THROW;
#endif

extern int chmod (const char *__file, mode_t __mode) __THROW;

extern int fchmod (int __fd, mode_t __mode) __THROW;

extern mode_t umask (mode_t __mask) __THROW;

extern int mkdir (const char *__path, mode_t __mode) __THROW;

extern int mknod (const char *__path, mode_t __mode, dev_t __dev) __THROW;

extern int mkfifo (const char *__path, mode_t __mode) __THROW;

#endif
