#include <sys/shm.h>

#include "dietpagesize.h"

struct __dirstream {
  int fd;
  unsigned int num;
  unsigned int cur;
  unsigned char is_64;
  char buf[] __attribute__((__aligned__(8)));
};				/* stream data from opendir() */

struct linux_dirent {
  unsigned long   d_ino;
  unsigned long   d_off;
  unsigned short  d_reclen;
  char            d_name[1];
};

struct linux_dirent64 {
  uint64_t        d_ino;
  int64_t         d_off;
  unsigned short  d_reclen;
  unsigned char   d_type;
  char            d_name[0];
};

#define __DIRSTREAM_BUF_SIZE	(__DIET_PAGE_SIZE - offsetof(struct __dirstream, buf))
