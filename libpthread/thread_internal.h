#ifndef __THREAD_INTERNAL_H__
#define __THREAD_INTERNAL_H__

#include <pthread.h>

/* cleanup */
#define PTHREAD_MAX_CLEANUP 4
struct thread_cleanup_t {
  void (*func)(void*);
  void *arg;
};

/* the thread descriptor / internal */
struct _pthread_descr_struct {
  /* runtime handling */
  struct _pthread_descr_struct *joined; /* a joined thread or NULL */

  /* thread/process data */
  int  pid;			/* Process id */

  int  policy;			/* thread scheduling policy */
  int  priority;		/* thread priority */

  /* errno handling */
  int   errno;
  int h_errno;

  /* stack handling */
  unsigned int stack_size;	/* stack size for setrlimit */
  void *stack_addr;		/* stack address for clone */
  void *stack_begin;		/* begin of stack / lowest address (free) */
  int  userstack;		/* user has provided the stack */

  /* thread exit handling */
  void  *retval;		/* thread return value */
  int  join;			/* thread waits for other to return */

  /* thread flags */
  int  detached;		/* thread is detached */
  int  canceled;		/* thread canceled */

  /* cancel handling */
  int  cancelstate;		/* cancel state */
  int  canceltype;		/* type of cancellation */

  /* thread basics */
  void* (*func) (void* arg);	/* thread function */
  void *arg;			/* thread argument */

  /* create thread / manager thread lock */
  struct _pthread_fastlock *manager_lock;

  /* cleanup stack */
  struct thread_cleanup_t cleanup_stack[PTHREAD_MAX_CLEANUP];

} __attribute__ ((aligned(32)));

int __testandset(int *spinlock);

void __pthread_lock(struct _pthread_fastlock * lock);
int __pthread_trylock(struct _pthread_fastlock * lock);
int __pthread_unlock(struct _pthread_fastlock * lock);

int __clone(void* (*fn)(void*), void* stack, int flags, void *arg);

int __find_thread_id(int pid);
_pthread_descr __get_thread_struct(int id);

_pthread_descr __thread_get_free();
_pthread_descr __thread_self();

void __thread_cleanup(_pthread_descr th);

void __thread_wait_some_time();

int __thread_create(void *(*__start_routine) (void *),
		void *__arg,
		char* stack,
		unsigned long stacksize,
		int detach,
		int inherit,
		int spolicy,
		int spriority);

/* manager thread stuff */
int signal_manager_thread(_pthread_descr td);

/* init stuff */
extern pthread_once_t __thread_inited;
void __thread_init();
#define __THREAD_INIT() __pthread_once(&__thread_inited, __thread_init)
#define __TEST_CNACEL() pthread_testcancel()

#endif
