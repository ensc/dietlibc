#include <linux/if.h>
#include <linux/net.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/socket.h>

#ifndef SOCK_DGRAM
#define SOCK_DGRAM 2
#endif

int if_nametoindex(char* blub) {
  struct ifreq ifr;
  int fd;
  char *tmp;
  int len=sizeof(ifr.ifr_name);
  fd=socket(AF_INET6,SOCK_DGRAM,0);
  if (fd<0) fd=socket(AF_INET,SOCK_DGRAM,0);
  for (tmp=ifr.ifr_name; len>0; --len) {
    if ((*tmp++=*blub++)==0) break;
  }
  if (ioctl(fd,SIOCGIFINDEX,&ifr)==0) {
    close(fd);
    return ifr.ifr_ifindex;
  }
  close(fd);
  return 0;
}
