#ifndef _PTHREAD_H
#define _PTHREAD_H 1

/* Most of this is taken from Linuxthreads */

//#include <linux/sched.h>

typedef struct _pthread_descr_struct *_pthread_descr;

/* Fast locks (not abstract because mutexes and conditions aren't abstract). */
struct _pthread_fastlock {
  int __status;			/* "Free" or "taken" or head of waiting list */
  int __spinlock;		/* For compare-and-swap emulation */
};

/* Mutexes (not abstract because of PTHREAD_MUTEX_INITIALIZER).  */
typedef struct {
  int reserved;			/* Reserved for future use */
  int count;			/* Depth of recursive locking */
  _pthread_descr owner;		/* Owner thread (if recursive or errcheck) */
  int kind;			/* Mutex kind: fast, recursive or errcheck */
  struct _pthread_fastlock lock;/* Underlying fast lock */
} pthread_mutex_t;

typedef struct {
  int __mutexkind;
} pthread_mutexattr_t;


/* Attributes for threads.  */
typedef struct
{
  int    __detachstate;
  int    __schedpolicy;
//  struct __sched_param __schedparam;
  int    __inheritsched;
  int    __scope;
  unsigned long __guardsize;
  int    __stackaddr_set;
  void*  __stackaddr;
  unsigned long __stacksize;
} pthread_attr_t;

typedef unsigned long int pthread_t;

int pthread_create (pthread_t *__thread,
		const pthread_attr_t *__attr,
		void *(*__start_routine) (void *),
		void *__arg);

void pthread_exit (void *__retval) __attribute__ ((__noreturn__));

int pthread_join (pthread_t __th, void **__thread_return);

int pthread_detach (pthread_t __th);

pthread_t pthread_self (void);
int pthread_equal (pthread_t __thread1, pthread_t __thread2);

int pthread_mutex_init(pthread_mutex_t *mutex,
		const pthread_mutexattr_t *mutexattr);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_destroy(pthread_mutex_t *mutex);

#endif
