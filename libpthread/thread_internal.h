#ifndef __THREAD_INTERNAL_H__
#define __THREAD_INTERNAL_H__

#include <pthread.h>
#include <stdarg.h>
#include <setjmp.h>
#include "dietfeatures.h"
#ifndef WANT_THREAD_SAFE
#error "the diet libc is not compiled with thread safeness enabled!"
#endif

#undef errno

/* cleanup */
#define PTHREAD_MAX_CLEANUP 8
struct thread_cleanup_t {
  void (*func)(void*);
  void *arg;
};

/* the thread descriptor / internal */
struct _pthread_descr_struct {
  /* conditional variables */
  struct _pthread_descr_struct *waitnext; /* an other waiting thread or NULL */
  int  waiting;			/* internal waiting "lock" */

  /* thread/process data */
  int  pid;			/* Process id */
  int  exited;			/* Process is dead */

  int  policy;			/* thread scheduling policy */
  int  priority;		/* thread priority */

  /* errno handling */
  int   errno;
  int h_errno;

  /* stack handling */
  unsigned int stack_size;	/* stack size for setrlimit */
  void *stack_addr;		/* stack address for clone */
  void *stack_begin;		/* begin of lib-stack / lowest address (free) */

  /* thread exit handling */
  void  *retval;		/* thread return value */
  int  joined;			/* flag other thread has joined */
  jmp_buf jmp_exit;		/* pthread_exit jump */

  /* thread flags */
  int  detached;		/* thread is detached */
  int  canceled;		/* thread canceled */

  /* cancel handling */
  int  cancelstate;		/* cancel state */
  int  canceltype;		/* type of cancellation */

  /* thread basics */
  void* (*func) (void* arg);	/* thread function */
  void* arg;			/* thread argument */

  /* create thread / manager thread lock */
  struct _pthread_fastlock go;

  /* cleanup stack */
  struct thread_cleanup_t cleanup_stack[PTHREAD_MAX_CLEANUP];

} __attribute__ ((aligned(32)));

/* thread keys */
struct _thread_key {
  int used;
  void (*destructor)(const void*);
  const void *tkd[PTHREAD_THREADS_MAX];
};

/* internal stuff */
int __testandset(int *spinlock);

void __pthread_lock(struct _pthread_fastlock * lock);
int __pthread_trylock(struct _pthread_fastlock * lock);
int __pthread_unlock(struct _pthread_fastlock * lock);

int __clone(void* (*fn)(void*), void* stack, int flags, void *arg);

int __find_thread_id(int pid);
_pthread_descr __get_thread_struct(int id);

_pthread_descr __thread_get_free(void);
_pthread_descr __thread_self(void);

int __thread_join(_pthread_descr join, void **return_value);
#define __thread_cleanup(th) (void)__thread_join((th),0)

void __thread_wait_some_time(void);

#define __NO_ASYNC_CANCEL_BEGIN { int oldtype; pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldtype);
#define __NO_ASYNC_CANCEL_END pthread_setcanceltype(oldtype,0); pthread_testcancel(); }
#define __NO_ASYNC_CANCEL_STOP pthread_setcanceltype(oldtype,0); }

/* manager thread stuff */
int signal_manager_thread(_pthread_descr td);

/* init stuff */
extern pthread_once_t __thread_inited;
void __thread_init(void);
#define __THREAD_INIT() __pthread_once(&__thread_inited, __thread_init)
#define __TEST_CANCEL() pthread_testcancel()

/* diet libc syscalls */

void  __libc_free(void *ptr);
void *__libc_malloc(size_t size);

void __libc_closelog(void);
void __libc_openlog(const char *ident, int option, int facility);
void __libc_vsyslog(int priority, const char *format, va_list arg_ptr);

pid_t __libc_fork(void);

int __libc_close(int fd);
int __libc_creat(const char *pathname, mode_t mode);
int __libc_fcntl(int fd, int cmd, void *arg);
int __libc_fsync(int fd);
int __libc_nanosleep(const struct timespec *req, struct timespec *rem);
int __libc_open(const char *pathname, int flags, mode_t mode);
int __libc_pause(void);
int __libc_read(int fd, void *buf, size_t count);
int __libc_sigsuspend(const sigset_t *mask);
int __libc_tcdrain(int fd);
pid_t __libc_waitpid(pid_t pid, int *status, int options);
int __libc_write(int fd, const void *buf, size_t count);

#endif
