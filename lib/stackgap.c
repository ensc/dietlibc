#include <unistd.h>
#include <fcntl.h>
#include <alloca.h>
#include <sys/time.h>
#include "dietfeatures.h"

extern int main(int argc,char* argv[],char* envp[]);

#ifdef WANT_SSP
extern unsigned long __guard;
#endif

int stackgap(int argc,char* argv[],char* envp[]);
int stackgap(int argc,char* argv[],char* envp[]) {
#ifdef WANT_SSP_XOR
  struct timeval tv;
#endif
#if defined(WANT_STACKGAP) || defined(WANT_SSP_URANDOM)
  int fd=open("/dev/urandom",O_RDONLY);
#ifdef WANT_STACKGAP
  unsigned short s;
  volatile char* gap;
  read(fd,&s,2);
#endif
#ifdef WANT_SSP
  read(fd,&__guard,sizeof(__guard));
#endif
  close(fd);
#ifdef WANT_STACKGAP
  gap=alloca(s);
#endif
#endif
#ifdef WANT_SSP_XOR
  gettimeofday (&tv, NULL);
  __guard ^= tv.tv_usec ^ tv.tv_sec ^ getpid();
#endif
  return main(argc,argv,envp);
}

