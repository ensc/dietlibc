#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "dietfeatures.h"

char* mkdtemp(char* template) {
  char *tmp=template+strlen(template)-6;
  int randfd,i;
  unsigned int random;
  for (i=0; i<6; ++i) if (tmp[i]!='X') { errno=EINVAL; return 0; }
  randfd=open("/dev/urandom",O_RDONLY);
  for (;;) {
    read(randfd,&random,sizeof(random));
    for (i=0; i<6; ++i) {
      int hexdigit=(random>>(i*5))&0x1f;
      tmp[i]=hexdigit>9?hexdigit+'a'-10:hexdigit+'0';
    }
    if (mkdir(template,0700)==-1) {
      if (errno==EEXIST) continue;
      close(randfd);
      return 0;
    }
  }
  close(randfd);
  return template;
}
