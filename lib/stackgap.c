#include <unistd.h>
#include <fcntl.h>
#include <alloca.h>
#include <sys/time.h>
#include <sys/tls.h>
#include "dietfeatures.h"

extern int main(int argc,char* argv[],char* envp[]);

#if defined(WANT_SSP)
extern unsigned long __guard;
#endif

#if defined(WANT_SSP) || defined(WANT_THREAD_SAFE)
static tcbhead_t mainthread;

static void setup_tls(void) {
#if defined(__x86_64__)

  mainthread.tcb=&mainthread;
  mainthread.self=&mainthread;
#if defined(WANT_SSP)
  mainthread.stack_guard=__guard;
#endif
  arch_prctl(ARCH_SET_FS, &mainthread);

#elif defined(__i386__)

  static unsigned int sd[4];
  mainthread.tcb=&mainthread;
  mainthread.self=&mainthread;
#if defined(WANT_SSP)
  mainthread.stack_guard=__guard;
#endif
  sd[0]=-1;
  sd[1]=(unsigned long int)&mainthread;
  sd[2]=0xfffff; /* 4 GB limit */
  sd[3]=0x51; /* bitfield, see struct user_desc in asm-i386/ldt.h */
  if (set_thread_area(&sd)==0) {
    asm volatile ("movw %w0, %%gs" :: "q" (sd[0]*8+3));
  }

#else
#warning "no idea how to enable TLS on this platform, edit lib/stackgap.c"
#endif
}
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

#if defined(WANT_SSP) || defined(WANT_THREAD_SAFE)
  setup_tls();
#endif
  return main(argc,argv,envp);
}

