#define _ATFILE_SOURCE
#include <errno.h>
#include "dietfeatures.h"
#include "syscalls.h"

#if defined(__NR_fstatat64) && defined(__NR_fstatat)

#ifdef WANT_LARGEFILE_BACKCOMPAT
#include <sys/stat.h>
#include <fcntl.h>
#ifndef __NO_STAT64

extern int __dietlibc_fstatat64(int dirfd, const char* pathname, struct stat64 *buf, int flags);
extern void __stat64_cvt(const struct stat *src,struct stat64 *dest);

int fstatat64(int dirfd, const char* pathname, struct stat64 *buf, int flags) {
  if (__dietlibc_fstatat64(dirfd,pathname,buf,flags)) {
    struct stat temp;
    if (errno!=ENOSYS) return -1;
    if (fstatat(dirfd,pathname,&temp,flags)) return -1;
    __stat64_cvt(&temp,buf);
  }
  return 0;
}
#endif
#endif

#elif defined(__NR_fstatat64) && !defined(__NR_fstatat)

#include <sys/stat.h>
#include <fcntl.h>
int fstatat(int dirfd, const char* pathname, struct stat *buf, int flags) {
  struct stat64 ss;
  if (fstatat64(dirfd,pathname,&ss,flags)==0) {
    buf->st_dev=ss.st_dev;
    buf->st_ino=ss.st_ino;
    buf->st_mode=ss.st_mode;
    buf->st_nlink=ss.st_nlink;
    buf->st_uid=ss.st_uid;
    buf->st_gid=ss.st_gid;
    buf->st_rdev=ss.st_rdev;
    buf->st_size=ss.st_size;
    buf->st_blksize=ss.st_blksize;
    buf->st_blocks=ss.st_blocks;
    buf->st_atime=ss.st_atime;
    buf->st_mtime=ss.st_mtime;
    buf->st_ctime=ss.st_ctime;
    if (buf->st_uid!=ss.st_uid || buf->st_gid!=ss.st_gid || buf->st_size!=ss.st_size || buf->st_ino!=ss.st_ino || buf->st_blocks!=ss.st_blocks)
      errno=EOVERFLOW;
  }
  return 0;
}

#endif
