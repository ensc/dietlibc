#ifndef _STDLIB_H
#define _STDLIB_H

#include <sys/cdefs.h>
#include <sys/types.h>

void *calloc(size_t nmemb, size_t size) __THROW;
void *malloc(size_t size) __THROW;
void free(void *ptr) __THROW;
void *realloc(void *ptr, size_t size) __THROW;

char *getenv(const char *name) __THROW __pure__;
int putenv(const char *string) __THROW;

int atexit(void (*function)(void)) __THROW;

double strtod(const char *nptr, char **endptr) __THROW;
long int strtol(const char *nptr, char **endptr, int base) __THROW;
unsigned long int strtoul(const char *nptr, char **endptr, int base) __THROW;

int __ltostr(char *s, int size, unsigned long i, int base, char UpCase) __THROW;
#ifdef __GNUC__
long long int strtoll(const char *nptr, char **endptr, int base) __THROW;
unsigned long long int strtoull(const char *nptr, char **endptr, int base) __THROW;
int __lltostr(char *s, int size, unsigned long long i, int base, char UpCase) __THROW;
#endif

int atoi(const char *nptr) __THROW;
long int atol(const char *nptr) __THROW;
double atof(const char *nptr) __THROW;

void exit(int status) __THROW __attribute__((noreturn));
void abort(void) __THROW;

/* warning: the rand() implementation of the diet libc really sucks. */
#define RAND_MAX 32767

extern int rand(void) __THROW;
extern void srand(unsigned int seed) __THROW;

void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) __THROW;
void *bsearch(const void *key, const void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) __THROW;

extern char **environ;

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

/* now this function is the greatest bullshit I have ever seen.
 * The ISO people must be out of their minds. */
typedef struct { int quot,rem; } div_t;
div_t div(int numer, int denom) __THROW __attribute__((const));

void *alloca(size_t size) __THROW; /* gcc built-in */


#endif
