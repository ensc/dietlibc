#ifndef _STDIO_H
#define _STDIO_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct __stdio_file FILE;

FILE *fopen (const char *path, const char *mode) __THROW;
FILE *fdopen (int fildes, const char *mode) __THROW;
FILE *freopen (const char *path, const char *mode, FILE *stream) __THROW;

int printf(const char *format, ...) __THROW;
int fprintf(FILE *stream, const char *format, ...) __THROW;
int sprintf(char *str, const char *format, ...) __THROW;
int snprintf(char *str, size_t size, const char *format, ...) __THROW;
int asprintf(char **ptr, const char* format, ...) __THROW;

int scanf(const char *format, ...) __THROW;
int fscanf(FILE *stream, const char *format, ...) __THROW;
int sscanf(const char *str, const char *format, ...) __THROW;

#include <stdarg.h>

int vprintf(const char *format, va_list ap) __THROW;
int vfprintf(FILE *stream, const char *format, va_list ap) __THROW;
int vsprintf(char *str, const char *format, va_list ap) __THROW;
int vsnprintf(char *str, size_t size, const char *format, va_list ap) __THROW;

int vscanf(const char *format, va_list ap) __THROW;
int vsscanf(const char *str, const char *format, va_list ap) __THROW;
int vfscanf(FILE *stream, const char *format, va_list ap) __THROW;

int fgetc(FILE *stream) __THROW;
int fgetc_unlocked(FILE *stream) __THROW;
char *fgets(char *s, int size, FILE *stream) __THROW;
int getc(FILE *stream) __THROW;
int getchar(void) __THROW;
char *gets(char *s) __THROW;
int ungetc(int c, FILE *stream) __THROW;

int fputc(int c, FILE *stream) __THROW;
int fputc_unlocked(int c, FILE *stream) __THROW;
int fputs(const char *s, FILE *stream) __THROW;

#define putc(c,stream) fputc(c,stream)
#define putchar(c) fputc(c,stdout)

#define getchar() fgetc(stdin)
#define getc(stream) fgetc(stream)
#define getc_unlocked(stream) fgetc_unlocked(stream)

int puts(const char *s) __THROW;

long fseek(FILE *stream, long offset, int whence) __THROW;
long ftell(FILE *stream) __THROW;
off_t fseeko(FILE *stream, off_t offset, int whence) __THROW;
off_t ftello(FILE *stream) __THROW;

#ifndef __NO_STAT64
loff_t fseeko64(FILE *stream, loff_t offset, int whence) __THROW;
loff_t ftello64(FILE *stream) __THROW;

#if _FILE_OFFSET_BITS == 64
#define off_t loff_t
#define fseeko(foo,bar,baz) fseeko64(foo,bar,baz)
#define ftello(foo) ftello64(foo)
#endif

#endif

void rewind(FILE *stream) __THROW;
int fgetpos(FILE *stream, fpos_t *pos) __THROW;
int fsetpos(FILE *stream, fpos_t *pos) __THROW;

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) __THROW;

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) __THROW;

int fflush(FILE *stream) __THROW;

int fclose(FILE *stream) __THROW;

int feof (FILE *stream) __THROW;
int ferror(FILE *stream) __THROW;
int fileno(FILE *stream) __THROW;
void clearerr(FILE *stream) __THROW;

int remove(const char *pathname) __THROW;

void perror(const char *s) __THROW;

extern FILE *stdin, *stdout, *stderr;

#define EOF (int)(-1)

#define BUFSIZ 128

#define _IONBF 0
#define _IOLBF 1
#define _IOFBF 2

int setvbuf(FILE *stream, char *buf, int mode , size_t size) __THROW;

#define setbuf(stream,buf) setvbuf(stream,buf,buf?_IOFBF:_IONBF,BUFSIZ)
#define setbuffer(stream,buf,size) setvbuf(stream,buf,buf?_IOFBF:_IONBF,size)
#define setlinebuf(stream) setvbuf(stream,0,_IOLBF,BUFSIZ)

FILE *popen(const char *command, const char *type) __THROW;
int pclose(FILE *stream) __THROW;

#endif
