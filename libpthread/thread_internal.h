#ifndef __THREAD_INTERNAL_H__
#define __THREAD_INTERNAL_H__

#include <pthread.h>

/* the thread descriptor / internal */
struct _pthread_descr_struct {
  /* runtime handling */
  struct _pthread_descr_struct * joined;	/* a joined thread or NULL */
  struct _pthread_descr_struct * lock;		/* next waiting thread */

  /* thread/process data */
  int  pid;					/* Process id */

  int  policy;					/* thread scheduling policy */
  int  priority;				/* thread priority */

  /* errno handling */
  int   errno;
  int h_errno;

  /* stack handling */
  unsigned int stack_size;			/* stack size for setrlimit */
  void *stack_begin;				/* begin of stack / lowest address (free) */
  int  userstack;				/* user has provided the stack */

  /* signal handling */

  /* thread exit handling */
  void  *retval;				/* thread return value */
  int  join;					/* thread waits for other to return */

  /* thread flags */
  int  detached;				/* thread is detached */
  int  canceled;				/* thread canceled */

  /* cancel handling */
  int  cancelstate;				/* cancel state */
  int  canceltype;				/* type of cancellation */

  /* thread basics */
  void* (*func) (void* arg);			/* thread function */
  void *arg;					/* thread argument */

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

void __thread_wait_some_time();

int __thread_create(void *(*__start_routine) (void *),
		void *__arg,
		char* stack,
		unsigned long stacksize,
		int detach,
		int inherit,
		int spolicy,
		int spriority);

/* init stuff */
extern pthread_once_t __thread_inited;
void __thread_init();
#define __THREAD_INIT() __pthread_once(&__thread_inited, __thread_init)
#define __TEST_CNACEL() pthread_testcancel()

#endif
