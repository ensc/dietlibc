int read(int fd, void* buf, int len);

struct iovec {
  void *iov_base;		/* Starting address */
  unsigned long iov_len;	/* Number of bytes */
};

int readv(int filedes, const struct iovec *vector, long count)
{
  int i,ret=0,r;
  for (i=0; i<count; i++)
  {
    if ((r=read(filedes,vector[i].iov_base,vector[i].iov_len))==-1) return r;
    ret+=r;
  }
  return ret;
}
