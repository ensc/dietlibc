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

typedef struct __file {
  int fd;
  int flags;
  unsigned int bs;	/* read: bytes in buffer */
  unsigned int bm;	/* position in buffer */
  char buf[BUFSIZE];
  struct __file *next;	/* for fflush */
  pid_t popen_kludge;
  char ungetbuf;
  char ungotten;
#ifdef WANT_THREAD_SAFE
  pthread_mutex_t m;
#endif
} FILE;

extern FILE *__stdio_root;

#define ERRORINDICATOR 1
#define EOFINDICATOR 2
#define BUFINPUT 4
#define BUFLINEWISE 8
#define NOBUF 16

#define _IONBF 0
#define _IOLBF 1
#define _IOFBF 2

/* internal function to flush buffer.
 * However, if next is BUFINPUT and the buffer is an input buffer, it
 * will not be flushed. Vice versa for output */
extern int __fflush4(FILE *stream,int next);

FILE *fopen (const char *path, const char *mode);
FILE *fdopen (int fildes, const char *mode);
FILE *freopen (const char *path, const char *mode, FILE *stream);

int fgetc(FILE *stream);
int fgetc_unlocked(FILE *stream) __THROW;
char *fgets(char *s, int size, FILE *stream);
int getc(FILE *stream);
int getchar(void);
char *gets(char *s);
int ungetc(int c, FILE *stream);

int fputc(int c, FILE *stream);
int fputc_unlocked(int c, FILE *stream) __THROW;
int fputs(const char *s, FILE *stream);
int putc(int c, FILE *stream);
int putchar(int c);
int puts(const char *s);

long fseek( FILE *stream, long offset, int whence);
long ftell( FILE *stream);
void rewind( FILE *stream);
int fgetpos( FILE *stream, fpos_t *pos);
int fsetpos( FILE *stream, fpos_t *pos);

size_t fread( void *ptr, size_t size, size_t nmemb, FILE *stream);

size_t fwrite( const void *ptr, size_t size, size_t nmemb, FILE *stream);

int fflush(FILE *stream);

int fclose(FILE *stream);

extern FILE *stdout, *stderr, *stdin;

#define EOF (int)(-1)

extern int __buffered_outs(const char *s,int len);

/* ..scanf */
struct arg_scanf {
  void *data;
  int (*getch)(void*);
  int (*putch)(int,void*);
};

int __v_scanf(struct arg_scanf* fn, const unsigned char *format, va_list arg_ptr);

