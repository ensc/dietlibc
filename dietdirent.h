struct __dirstream {
  int fd;
  char buf[1024];
  unsigned int num;
  unsigned int cur;
};				/* stream data from opendir() */
