#ifndef _STDIO_H
#define _STDIO_H

#include <sys/types.h>

typedef struct __stdio_file FILE;

FILE *fopen (const char *path, const char *mode);
FILE *fdopen (int fildes, const char *mode);
FILE *freopen (const char *path, const char *mode, FILE *stream);

int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);

int scanf( const char *format, ...);
int fscanf( FILE *stream, const char *format, ...);
int sscanf( const char *str, const char *format, ...);

#include <stdarg.h>

int vprintf(const char *format, va_list ap);
int vfprintf(FILE *stream, const char *format, va_list ap);
int vsprintf(char *str, const char *format, va_list ap);
int vsnprintf(char *str, size_t size, const char *format, va_list ap);

int vscanf( const char *format, va_list ap);
int vsscanf( const char *str, const char *format, va_list ap);
int vfscanf( FILE *stream, const char *format, va_list ap);

int fgetc(FILE *stream);
char *fgets(char *s, int size, FILE *stream);
int getc(FILE *stream);
int getchar(void);
char *gets(char *s);
int ungetc(int c, FILE *stream);

int fputc(int c, FILE *stream);
int fputs(const char *s, FILE *stream);

#define putc(c,stream) fputc(c,stream)
#define putchar(c) fputc(c,stdout)

#define getchar() fgetc(stdin)

int puts(const char *s);

int fseek( FILE *stream, long offset, int whence);
long ftell( FILE *stream);
void rewind( FILE *stream);
int fgetpos( FILE *stream, fpos_t *pos);
int fsetpos( FILE *stream, fpos_t *pos);

size_t fread( void *ptr, size_t size, size_t nmemb, FILE *stream);

size_t fwrite( const void *ptr, size_t size, size_t nmemb, FILE *stream);

int fflush(FILE *stream);

int fclose(FILE *stream);

int feof (FILE *stream);

extern FILE *stdin, *stdout, *stderr;

#define EOF (int)(-1)

#endif
