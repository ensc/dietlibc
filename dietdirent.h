#include <sys/shm.h>

struct __dirstream {
  char buf[PAGE_SIZE-(sizeof (int)*3)];
  int fd;
  unsigned int num;
  unsigned int cur;
};				/* stream data from opendir() */
