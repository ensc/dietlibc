#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#include <stdio.h>
#include "thread_internal.h"

static struct _pthread_descr_struct threads[PTHREAD_THREADS_MAX];
pthread_once_t __thread_inited;


/* find thread */
int __find_thread_id(int pid)
{
  register int i;
  for (i=0; i<PTHREAD_THREADS_MAX; i++)
    if (threads[i].pid==pid)
      return i;
  return -1;
}

/* get thread */
_pthread_descr __get_thread_struct(int id)
{
  return threads+id;
}

/* thread errno location */
int *__errno_location(void)
{
  int id=0;
  if (__thread_inited) id=__find_thread_id(getpid());
  if (id<0)
    return 0;
  else
    return &threads[id].errno;
}

/* thread self */
_pthread_descr __thread_self()
{
  register int i=__find_thread_id(getpid());
  if (i<0)
    return 0;
  else
    return threads+i;
}

/* SIGHUP handler (thread cnacel) PTHREAD_CANCEL_ASYNCHRONOUS */
static void __thread_cancel_handler(int sig)
{
  _pthread_descr this;
  this = __thread_self();
  if (this->canceltype==PTHREAD_CANCEL_ASYNCHRONOUS)
    pthread_exit(PTHREAD_CANCELED);
  signal( SIGHUP, __thread_cancel_handler );
}

/* SIGCHLD handler (thread exited) free thread struct */
static void __thread_slaughter(int sig)
{
  int pid, status, i;

  while(1)
  {
    pid = waitpid (0, &status, WNOHANG);
    if (pid <= 0) break;
    for (i=0; i<PTHREAD_THREADS_MAX; i++)
    {
      if (threads[i].pid==pid)
      {
	if (!threads[i].userstack)
	  free(threads[i].stack_begin);

	/* wake joined thread and put retval */
	if (threads[i].joined) {
	  threads[i].joined->retval=threads[i].retval;
	  threads[i].joined->join=0;
	}

	threads[i].pid=0;	// free to use
	break;
      }
    }
  }
  signal( SIGCHLD, __thread_slaughter );
}


/* sleep a little (reschedule for this time) */
void __thread_wait_some_time()
{
  struct timespec reg;
  reg.tv_sec=0;
  reg.tv_nsec=SPIN_SLEEP_DURATION;
  nanosleep(&reg,0);
}

/* thread stop */
static void __thread_main_exit()
{
  int i;
  unsigned int c=1;

  if (getpid()!=threads[0].pid)
  {
    printf("A THREAD ?\n");
    kill(threads[0].pid, SIGTERM);
    while(1) __thread_wait_some_time();
  }
  else
    printf("EXIT ! %d\n",getpid());

  for (i=1; i<PTHREAD_THREADS_MAX; i++)
  {
    if (threads[i].pid>1)
    {
      threads[i].canceled=1;
      printf("CANCEL ! %d\n",threads[i].pid);
      c++;
    }
  }

  while(--c) {
    sched_yield();
    __thread_wait_some_time();
  }

  for (i=1; i<PTHREAD_THREADS_MAX; i++)
  {
    if (threads[i].pid>1)
    {
      printf("KILL ! %d\n",threads[i].pid);
      kill(threads[i].pid, SIGTERM);
    }
  }
}

/* allocate a thread slot */
_pthread_descr __thread_get_free()
{
  int i;

  for (i=0; i<PTHREAD_THREADS_MAX; i++)
    if (threads[i].pid==0)
      if (!(__testandset(&threads[i].pid))) // atomic get slot :)
	return threads+i;

  return 0;
}

/* thread intern init */
void __thread_init()
{
  printf("INIT ! %d\n",getpid());
  memset(threads,0,sizeof(threads));

  signal( SIGCHLD, __thread_slaughter );
  signal( SIGHUP, __thread_cancel_handler );

  threads[0].pid = getpid();

  atexit(__thread_main_exit);
}

