/* this bullshit is here for glibc compatibility */

#include <sys/stat.h>

typedef unsigned long long __dev_t;
typedef unsigned long int __ino_t;
typedef unsigned long int __mode_t;
typedef unsigned long int __nlink_t;
typedef long int __off_t;
typedef unsigned long int __uid_t;
typedef unsigned long int __gid_t;
typedef long int __blksize_t;
typedef long int __blkcnt_t;
typedef long int __time_t;

struct glibc_stat
  {
    __dev_t st_dev;			/* Device.  */
    unsigned short int __pad1;
    __ino_t st_ino;			/* File serial number.	*/
    __mode_t st_mode;			/* File mode.  */
    __nlink_t st_nlink;			/* Link count.  */
    __uid_t st_uid;			/* User ID of the file's owner.	*/
    __gid_t st_gid;			/* Group ID of the file's group.*/
    __dev_t st_rdev;			/* Device number, if device.  */
    unsigned short int __pad2;
    __off_t st_size;			/* Size of file, in bytes.  */
    __blksize_t st_blksize;		/* Optimal block size for I/O.  */

    __blkcnt_t st_blocks;		/* Number 512-byte blocks allocated. */
    __time_t st_atime;			/* Time of last access.  */
    unsigned long int __unused1;
    __time_t st_mtime;			/* Time of last modification.  */
    unsigned long int __unused2;
    __time_t st_ctime;			/* Time of last status change.  */
    unsigned long int __unused3;
    unsigned long int __unused4;
    unsigned long int __unused5;
  };


void __stat_convert(struct glibc_stat *d,struct stat *s) {
  d->st_dev=s->st_dev;
  d->st_ino=s->st_ino;
  d->st_mode=s->st_mode;
  d->st_nlink=s->st_nlink;
  d->st_uid=s->st_uid;
  d->st_gid=s->st_gid;
  d->st_rdev=s->st_rdev;
  d->st_size=s->st_size;
  d->st_blksize=s->st_blksize;
  d->st_blocks=s->st_blocks;
  d->st_atime=s->st_atime;
  d->st_mtime=s->st_mtime;
  d->st_ctime=s->st_ctime;
}
