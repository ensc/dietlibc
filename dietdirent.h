#include <sys/shm.h>

struct __dirstream {
  char buf[PAGE_SIZE-(sizeof (int)*3)];
  int fd;
  unsigned int num;
  unsigned int cur;
  unsigned char is_64;
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
