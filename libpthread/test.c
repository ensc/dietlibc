#include <stdio.h>
#include <pthread.h>

#include <unistd.h>
#include <signal.h>

#include <errno.h>

int ra;

void test(int sig)
{
  printf("SIGNAL %d in %d\n",sig,getpid());
  fflush(stdout);
  signal(SIGHUP, test);
}

void* thread(void* arg)
{
  int i;
  for (i=0;i<10; i++)
  {
    printf("child %ld %8p\n", pthread_self(),arg);
    if ((long)arg)
    {
#if 0
      if (i>1)
      {
#if 0
	// SIGFPE
	int a=0;
	a=2/a;
#endif
#if 0
	// SIGSEGV
	char *tmp=i;
	*tmp=1;
#endif
//	if (ra) raise(ra);
	raise(SIGHUP);
      }
#endif
//      if (i>3) exit(0);
    }
    sleep(1);
  }
  return 0;
}

int main(int argc, char*argv[])
{
  pthread_t t;
  int i;

#if 0
  printf("%s\n",argv[1]);
  ra = atoi (argv[1]);
#endif

  signal(SIGHUP, test);

  pthread_create(&t,0, thread, 0);
  pthread_create(&t,0, thread, (void *)1);

  puts("main");

#if 0
  for(i=0;i<4;i++) sleep(1);
#else
  {
    int *tt;
    int i = pthread_join(t,(void*)&tt);
    printf("join %d %d %d\n",i,tt,(*(__errno_location())));
  }
#endif

  puts("main exit");

  return 0;
}
