#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

#define PTHREAD_NUM_ATFORK 4

struct {
  int used;
  void (*prepare)(void);
  void (*parent)(void);
  void (*child)(void);
} pthread_atfork_buf[PTHREAD_NUM_ATFORK];

int pthread_atfork(void (*prepare)(void),
		   void (*parent)(void),
		   void (*child)(void))
{
  int i;
  for (i=0;i<PTHREAD_NUM_ATFORK; ++i)
    if (!(pthread_atfork_buf[i].used))
      if (__testandset(&pthread_atfork_buf[i].used)) {
	pthread_atfork_buf[i].prepare=prepare;
	pthread_atfork_buf[i].parent=parent;
	pthread_atfork_buf[i].child=child;
	return 0;
      }
  (*(__errno_location()))=ENOMEM;
  return -1;
}

pid_t fork(void)
{
  pid_t pid;
  int i=PTHREAD_NUM_ATFORK;

  __TEST_CANCEL();

  do {
    --i;
    if (pthread_atfork_buf[i].used)
      if (pthread_atfork_buf[i].prepare)
	pthread_atfork_buf[i].prepare();
  } while (i>0);

  pid=__libc_fork();
  if (pid>0) {
    for (i=0;i<PTHREAD_NUM_ATFORK;++i)
      if (pthread_atfork_buf[i].used)
	if (pthread_atfork_buf[i].parent)
	  pthread_atfork_buf[i].parent();
  } else if (pid==0) {
    for (i=0;i<PTHREAD_NUM_ATFORK;++i)
      if (pthread_atfork_buf[i].used)
	if (pthread_atfork_buf[i].child)
	  pthread_atfork_buf[i].child();
  }

  return pid;
}
