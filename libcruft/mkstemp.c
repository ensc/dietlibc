#include <unistd.h>
#include <sys/fcntl.h>
#include <string.h>
#include <errno.h>

int mkstemp(char* template) {
  char *tmp=template+strlen(template)-6;
  int randfd;
  int i,res;
  unsigned int random;
  for (i=0; i<6; ++i) if (tmp[i]!='X') { __set_errno(EINVAL); return -1; }
  randfd=open("/dev/urandom",O_RDONLY);
  for (;;) {
    read(randfd,&random,sizeof(random));
    for (i=0; i<6; ++i) {
      tmp[i]=((random>>(i*4))&0xf)+'0';
      if (tmp[i]>'9') tmp[i]+='a'-'0';
    }
    puts(template);
    res=open(template,O_CREAT|O_RDWR|O_EXCL,0600);
    if (res>=0) return res;
  }
  close(randfd);
}
