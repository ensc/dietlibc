#include <unistd.h>
#include <fcntl.h>
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
      int hexdigit=(random>>(i*5))&0x1f;
      tmp[i]=hexdigit>9?hexdigit+'a'-10:hexdigit+'0';
    }
    res=open(template,O_CREAT|O_RDWR|O_EXCL,0600);
    if (res>=0) break;
  }
  close(randfd);
  return res;
}
