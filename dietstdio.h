/* diet stdio -- no buffering ;-} */

#include <sys/cdefs.h>
#include <sys/types.h>
#include "dietfeatures.h"
#ifdef WANT_THREAD_SAFE
#include <pthread.h>
#endif
#include <stdarg.h>

#ifdef WANT_SMALL_STDIO_BUFS
#define BUFSIZE 128
#else
#define BUFSIZE 2048
#endif

struct __stdio_file {
  int fd;
  int flags;
  unsigned int bs;	/* read: bytes in buffer */
  unsigned int bm;	/* position in buffer */
  unsigned int buflen;	/* length of buf */
  char *buf;
  struct __stdio_file *next;	/* for fflush */
  pid_t popen_kludge;
  unsigned char ungetbuf;
  char ungotten;
#ifdef WANT_THREAD_SAFE
  pthread_mutex_t m;
#endif
};

#define ERRORINDICATOR 1
#define EOFINDICATOR 2
#define BUFINPUT 4
#define BUFLINEWISE 8
#define NOBUF 16
#define STATICBUF 32

#define _IONBF 0
#define _IOLBF 1
#define _IOFBF 2

#include <stdio.h>

/* internal function to flush buffer.
 * However, if next is BUFINPUT and the buffer is an input buffer, it
 * will not be flushed. Vice versa for output */
extern int __fflush4(FILE *stream,int next);
extern int __buffered_outs(const char *s,size_t len);

/* ..scanf */
struct arg_scanf {
  void *data;
  int (*getch)(void*);
  int (*putch)(int,void*);
};

int __v_scanf(struct arg_scanf* fn, const unsigned char *format, va_list arg_ptr);

struct arg_printf {
  void *data;
  int (*put)(void*,size_t,void*);
};

int __v_printf(struct arg_printf* fn, const unsigned char *format, va_list arg_ptr);

extern FILE *__stdio_root;

int __fflush_stdin(void);
int __fflush_stdout(void);
int __fflush_stderr(void);

