#ifndef _PTHREAD_H
#define _PTHREAD_H 1

/* Most of this is taken from Linuxthreads */

#include <sched.h>
#include <signal.h>
#include <setjmp.h>

#define PTHREAD_THREADS_MAX	128

#define MAX_SPIN_COUNT		50
#define SPIN_SLEEP_DURATION	2000001

/* Fast locks (not abstract because mutexes and conditions aren't abstract). */
struct _pthread_fastlock {
  int __status;			/* "Free" or "taken" or head of waiting list */
  int __spinlock;		/* For compare-and-swap emulation */
};

typedef struct _pthread_descr_struct *_pthread_descr;

/* Mutexes (not abstract because of PTHREAD_MUTEX_INITIALIZER).  */
typedef struct {
  int reserved;			/* Reserved for future use */
  int count;			/* Depth of recursive locking */
  _pthread_descr owner;		/* Owner thread (if recursive or errcheck) */
  int kind;			/* Mutex kind: fast, recursive or errcheck */
  struct _pthread_fastlock lock;/* Underlying fast lock */
} pthread_mutex_t;

#define PTHREAD_MUTEX_INITIALIZER	{0, 0, 0, 0, {0, 0}}

typedef struct {
  int __mutexkind;
} pthread_mutexattr_t;


/* Attributes for threads.  */
typedef struct
{
  int		__detachstate;
  int		__schedpolicy;
  struct sched_param	__schedparam;
  int		__inheritsched;
  int		__scope;
  unsigned long __guardsize;
  int		__stackaddr_set;
  void *	__stackaddr;
  unsigned long __stacksize;
} pthread_attr_t;

typedef unsigned long int pthread_t;

/* ONCE */
typedef int pthread_once_t;
#define PTHREAD_ONCE_INIT	0

int __pthread_once(pthread_once_t* once_control, void (*init_routine)(void));
int pthread_once(pthread_once_t* once_control, void (*init_routine)(void));

/* CANCEL */

enum {
  PTHREAD_CANCEL_DISABLE,
#define PTHREAD_CANCEL_DISABLE PTHREAD_CANCEL_DISABLE
  PTHREAD_CANCEL_ENABLE,
#define PTHREAD_CANCEL_ENABLE PTHREAD_CANCEL_ENABLE
};

enum {
  PTHREAD_CANCEL_ASYNCHRONOUS,
#define PTHREAD_CANCEL_ASYNCHRONOUS PTHREAD_CANCEL_ASYNCHRONOUS
  PTHREAD_CANCEL_DEFERRED,
#define PTHREAD_CANCEL_DEFERRED PTHREAD_CANCEL_DEFERRED
};

#define PTHREAD_CANCELED ((void *) -1)

int pthread_cancel(pthread_t thread);
int pthread_setcancelstate(int state, int *oldstate);

int pthread_setcanceltype(int type, int *oldtype);

void pthread_testcancel(void);


/* THREADS */
int pthread_create (pthread_t *__thread,
		const pthread_attr_t *__attr,
		void *(*__start_routine) (void *),
		void *__arg);

void pthread_exit (void *__retval) __attribute__ ((__noreturn__));

int pthread_join (pthread_t __th, void **__thread_return);

int pthread_detach (pthread_t __th);

pthread_t pthread_self (void);
int pthread_equal (pthread_t __thread1, pthread_t __thread2);

/* MUTEX */
int pthread_mutex_init(pthread_mutex_t *mutex,
		const pthread_mutexattr_t *mutexattr);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_destroy(pthread_mutex_t *mutex);

#endif
