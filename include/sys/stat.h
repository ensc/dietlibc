#ifndef _SYS_STAT_H
#define _SYS_STAT_H

#include <sys/cdefs.h>
#include <sys/types.h>

#include <linux/stat.h>
#include <asm/stat.h>

#ifndef STAT64_HAS_BROKEN_ST_INO
#define __NO_STAT64
#endif

extern int stat(const char *__file, struct stat *__buf) __THROW;
extern int fstat(int __fd, struct stat *__buf) __THROW;
extern int lstat(const char *__file, struct stat *__buf) __THROW;

#ifdef __NO_STAT64
#undef _FILE_OFFSET_BITS
#else
extern int stat64(const char *__file, struct stat64 *__buf) __THROW;
extern int fstat64(int __fd, struct stat64 *__buf) __THROW;
extern int lstat64(const char *__file, struct stat64 *__buf) __THROW;
#endif

#if _FILE_OFFSET_BITS == 64
#define lstat(file,buf) lstat64(file,buf)
#define fstat(file,buf) fstat64(file,buf)
#define stat stat64
#endif

#define major(dev) (((dev)>>8) & 0xff)
#define minor(dev) ((dev) & 0xff)
#define makedev(major, minor) ((((unsigned int) (major)) << 8) | ((unsigned int) (minor)))

extern int chmod (const char *__file, mode_t __mode) __THROW;

extern int fchmod (int __fd, mode_t __mode) __THROW;

extern mode_t umask (mode_t __mask) __THROW;

extern int mkdir (const char *__path, mode_t __mode) __THROW;

extern int mknod (const char *__path, mode_t __mode, dev_t __dev) __THROW;

extern int mkfifo (const char *__path, mode_t __mode) __THROW;

#endif
