#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include <sched.h>
#include <sys/resource.h>

#include <stdio.h>
#include "thread_internal.h"

static struct _pthread_fastlock __thread_struct_lock = {0};
static struct _pthread_descr_struct threads[PTHREAD_THREADS_MAX];
static int _max_used_thread_id=1;
pthread_once_t __thread_inited;

static struct _pthread_fastlock __manager_thread_signal_lock = {0};
static struct _pthread_fastlock __manager_thread_data_lock = {1};
static struct _pthread_fastlock __manager_thread_data_go_lock = {1};

//#define DEBUG

/* find thread */
int __find_thread_id(int pid)
{
  register int i;
  for (i=0; i<_max_used_thread_id; i++)
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

/* allocate a thread slot */
_pthread_descr __thread_get_free()
{
  _pthread_descr ret=0;
  int i;

  __NO_ASYNC_CANCEL_BEGIN;
  __pthread_lock(&__thread_struct_lock);

  for (i=0; i<PTHREAD_THREADS_MAX; i++) {
    if (threads[i].pid==0) {
      ret = threads+i;
      /* clear struct */
      memset(ret,0,sizeof(struct _pthread_descr_struct));
      ret->pid=1; /* mark as taken */
      if (i>=_max_used_thread_id) _max_used_thread_id=i+1;
      break;
    }
  }

  __pthread_unlock(&__thread_struct_lock);
  __NO_ASYNC_CANCEL_END;
  return ret;
}

/* sleep a little (reschedule for this time) */
void __thread_wait_some_time()
{
  struct timespec reg;
  reg.tv_sec=0;
  reg.tv_nsec=SPIN_SLEEP_DURATION;
  __libc_nanosleep(&reg,0);
}

/* cleanup/join a thread */
int __thread_join(_pthread_descr th, void**return_value)
{
  /* mark thread th as joined */
  if (__testandset(&(th->joined))) return EINVAL;
  /* wait for thread to exit */
  while(!th->exited) __thread_wait_some_time();
  /* put return value to caller */
  if (return_value) *return_value=th->retval;
  /* cleanup thread */
  if (th->stack_begin) free(th->stack_begin);
  th->joined=0;
  th->pid=0;
  return 0;
}

/* SIGHUP handler (thread cancel) PTHREAD_CANCEL_ASYNCHRONOUS */
static void __thread_cancel_handler(int sig)
{
  _pthread_descr this;
  this = __thread_self();
  this->canceled=1;
  if (this->canceltype==PTHREAD_CANCEL_ASYNCHRONOUS)
    pthread_exit(PTHREAD_CANCELED);
  signal( SIGHUP, __thread_cancel_handler );
}

/* kill ALL threads / other then prime task and manager thread */
static void __kill_all_threads()
{
  int i;

  for (i=2; i<_max_used_thread_id; i++) {
    if (threads[i].pid>1) {
#ifdef DEBUG
      printf("CANCEL ! %d\n",threads[i].pid);
#endif
      threads[i].canceled=1;
      kill(threads[i].pid, SIGHUP);	/* cancel thread */
    }
  }

  __thread_wait_some_time();

  for (i=2; i<_max_used_thread_id; i++) {
    if (threads[i].pid>1) {
#ifdef DEBUG
      printf("KILL ! %d\n",threads[i].pid);
#endif
      kill(threads[i].pid, SIGTERM);	/* KILL thread */
    }
  }
}

__attribute__((weak)) volatile void __thread_start__key(int id);
__attribute__((weak)) volatile void __thread_start__key(int id) { return; }
__attribute__((weak,alias("__thread_start__key"))) volatile void __thread_exit__key(int id);

/* support for manager */
static void *__mthread_starter(void *arg)
{
  _pthread_descr td = (_pthread_descr)arg;
  int i = td->stack_size-4096;

  /* just to be sure */
  td->pid=getpid();

  /* signal handling for a thread */
  signal(SIGTERM, _exit);
  signal(SIGCHLD, SIG_DFL);
  signal(SIGHUP, __thread_cancel_handler );

  /* limit stack so that we NEVER have to worry */
  setrlimit(RLIMIT_STACK, (struct rlimit *)(&i));

  /* set scheduler */
  if (td->policy!=SCHED_OTHER) {
    struct sched_param sp;
    sp.sched_priority=td->priority;
    sched_setscheduler(td->pid,td->policy, &sp);
  }

  /* thread_key glue */
  __thread_start__key(td-threads);

#ifdef DEBUG
  printf("in starter %d, parameter %8p\n", td->pid, td->func);
#endif
  do {
    __thread_wait_some_time();
    if (td->canceled) return (void*)42;
  } while (__pthread_trylock(&td->go));

#ifdef DEBUG
  printf("post starter %d, parameter %8p\n", td->pid, td->func);
#endif

  if (!td->canceled) {
    if (!(setjmp(td->jmp_exit))) {
      td->retval=td->func(td->arg);
#ifdef DEBUG
    } else {
      printf("pthread_exit called in %d\n", td->pid);
#endif
    }
  }
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,0);

  /* thread_key glue */
  __thread_exit__key(td-threads);

#ifdef DEBUG
  printf("end starter %d, retval %8p\n", td->pid, td->retval);
#endif

  /* execute all functions on the cleanup-stack */
  for (i=PTHREAD_MAX_CLEANUP;i;) {
    if (td->cleanup_stack[--i].func) {
      td->cleanup_stack[i].func(td->cleanup_stack[i].arg);
    }
  }

  return 0;
}


/* manager thread and signal handler */
static char __manager_thread_stack[12*1024];
static volatile _pthread_descr __manager_thread_data;
static void __manager_SIGCHLD(int sig)
{
  int pid, status, i;

  while(1) {
    pid = __libc_waitpid (-1, &status, WNOHANG);
    if (pid <= 0) break;

    for (i=0; i<_max_used_thread_id; i++) {
      if (threads[i].pid==pid) {
	++threads[i].exited;
	if (threads[i].detached) {
	  threads[i].joined=0;
	  __thread_cleanup(threads+i);
	}
	break;
      }
    }
  }
}

static void __manager_SIGTERM(int sig)
{
  __kill_all_threads();
  _exit(0);
}

static void* __manager_thread(void *arg)
{
  struct sigaction sig_action_chld;
  sig_action_chld.sa_handler = __manager_SIGCHLD;
  sigemptyset(&sig_action_chld.sa_mask);
  sig_action_chld.sa_flags = SA_RESTART;

  sigaction(SIGCHLD, &sig_action_chld, 0);
  signal(SIGTERM, __manager_SIGTERM);
  signal(SIGHUP, SIG_IGN);

  __pthread_unlock(&__manager_thread_data_go_lock);	/* release init */
  while(1) {
    do {
      __thread_wait_some_time();
      if (getppid()<2) __manager_SIGTERM(0);
    } while (__pthread_trylock(&__manager_thread_data_lock));

    __manager_thread_data->pid =
      __clone(__mthread_starter,
	      __manager_thread_data->stack_addr,
	      CLONE_VM | CLONE_FS | CLONE_FILES | SIGCHLD,
	      __manager_thread_data);
    __thread_wait_some_time();
    __thread_wait_some_time();
    __pthread_unlock(&__manager_thread_data->go);
#ifdef DEBUG
    printf("manager new thread %d\n",__manager_thread_data->pid);
#endif
    __pthread_unlock(&__manager_thread_data_go_lock);	/* release sender */
  }
  return 0;
}

/* pthread_create bottom half */
int signal_manager_thread(_pthread_descr td)
{
  __NO_ASYNC_CANCEL_BEGIN;

  __pthread_lock(&__manager_thread_signal_lock);	/* lock */

  __manager_thread_data = td;
  __thread_wait_some_time();
  __pthread_unlock(&__manager_thread_data_lock);	/* signal manager to start */
  __thread_wait_some_time();
  __pthread_lock(&__manager_thread_data_go_lock);	/* wait for manager */

  __pthread_unlock(&__manager_thread_signal_lock);	/* unlock */

  __NO_ASYNC_CANCEL_END;

  return td->pid;
}


/* thread stop */
static void __thread_main_exit()
{
  if (getpid()!=threads[0].pid) {
#ifdef DEBUG
    printf("A THREAD ? %d\n",getpid());
#endif
    kill(threads[0].pid, SIGTERM);
    while(1) __thread_wait_some_time();
  }
#ifdef DEBUG
  else
    printf("EXIT ! %d\n",getpid());
#endif

  /* stop ALL threads */
  kill(threads[1].pid, SIGTERM);
  __thread_wait_some_time();
  __kill_all_threads();
}

/* thread intern init */
void __thread_init()
{
  if (atexit(__thread_main_exit)==-1)
    exit(42);

#ifdef DEBUG
  printf("INIT ! %d\n",getpid());
  memset(threads,0,sizeof(threads));
#endif

  threads[0].pid = getpid();

  ++_max_used_thread_id;
  threads[1].stack_size=sizeof(__manager_thread_stack);
  threads[1].stack_addr=&__manager_thread_stack[sizeof(__manager_thread_stack)];
  threads[1].stack_begin=0;
  threads[1].func=__manager_thread;

  threads[1].pid = __clone(__mthread_starter, threads[1].stack_addr,
			   CLONE_VM | CLONE_FS | CLONE_FILES, threads+1);

#ifdef DEBUG
  printf("manager thread @ : %d\n",threads[1].pid);
#endif
  __pthread_lock(&__manager_thread_data_go_lock);	/* wait for manager to be ready */
}

