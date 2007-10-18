#ifndef _SYS_STATVFS_H
#define _SYS_STATVFS_H

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct statvfs {
  unsigned long  f_bsize;    /* file system block size */
  unsigned long  f_frsize;   /* fragment size */
  fsblkcnt_t     f_blocks;   /* size of fs in f_frsize units */
  fsblkcnt_t     f_bfree;    /* # free blocks */
  fsblkcnt_t     f_bavail;   /* # free blocks for non-root */
  fsfilcnt_t     f_files;    /* # inodes */
  fsfilcnt_t     f_ffree;    /* # free inodes */
  fsfilcnt_t     f_favail;   /* # free inodes for non-root */
  unsigned long  f_fsid;     /* file system ID */
  unsigned long  f_flag;     /* mount flags */
  unsigned long  f_namemax;  /* maximum filename length */
};

int statvfs(const char *path, struct statvfs *buf) __THROW;
int fstatvfs(int fd, struct statvfs *buf) __THROW;

__END_DECLS

#endif
