int write(int fd, void *buf, int len);

struct iovec {
  void *iov_base;		/* Starting address */
  unsigned long iov_len;	/* Number of bytes */
};

int writev(int filedes, const struct iovec *vector, long count)
{
  int i,ret=0,r;
  for (i=0; i<count; i++)
  {
    if ((r=write(filedes,vector[i].iov_base,vector[i].iov_len))==-1) return ret;
    ret+=r;
  }
  return ret;
}
