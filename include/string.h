#ifndef _STRING_H
#define _STRING_H

#include <sys/cdefs.h>
#include <sys/types.h>

char *strcpy(char *dest, const char *src) __THROW;
char *strncpy(char *dest, const char *src, size_t n) __THROW;

void *memccpy(void *dest, const void *src, int c, size_t n) __THROW;
void *memcpy(void *dest, const void *src, size_t n) __THROW;
void *memmove(void *dest, const void *src, size_t n) __THROW;

int memccmp(const void *s1, const void *s2, int c, size_t n) __THROW __pure__;
int memcmp(const void *s1, const void *s2, size_t n) __THROW __pure__;
int strcmp(const char *s1, const char *s2) __THROW __pure__;
int strncmp(const char *s1, const char *s2, size_t n) __THROW __pure__;

int strcasecmp(const char *s1, const char *s2) __THROW __pure__;
int strncasecmp(const char *s1, const char *s2, size_t n) __THROW __pure__;

size_t strlen(const char *s) __THROW __pure__;

char *strstr(const char *haystack, const char *needle) __THROW __pure__;

char *strdup(const char *s) __THROW;

char *strchr(const char *s, int c) __THROW __pure__;
char *strrchr(const char *s, int c) __THROW __pure__;

char *strcat(char *dest, const char *src) __THROW;
char *strncat(char *dest, const char *src, size_t n) __THROW;

size_t strspn(const char *s, const char *_accept) __THROW;
size_t strcspn(const char *s, const char *reject) __THROW;

char *strpbrk(const char *s, const char *_accept) __THROW;
char *strsep(char **stringp, const char *delim) __THROW;

void* memset(void *s, int c, size_t n) __THROW;
void* memchr(const void *s, int c, size_t n) __THROW;

/* I would like to make this const, but Paul Jarc points out it has to
 * be char* :-( */
char *strerror(int errnum) __THROW;
/* work around b0rken GNU crapware like tar 1.13.19 */
#define strerror strerror

#ifdef _GNU_SOURCE
char *strsignal(int signum) __THROW;
void *memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen) __THROW;
#endif

char *strtok(char *s, const char *delim) __THROW;
char *strtok_r(char *s, const char *delim, char **ptrptr) __THROW;

int ffs(int i) __THROW __attribute__((__const__));

size_t strlcpy(char *dst, const char *src, size_t size) __THROW;
size_t strlcat(char *dst, const char *src, size_t size) __THROW;

int strcoll(const char *s1, const char *s2) __THROW;

#ifdef _BSD_SOURCE
#define bzero(s,n) memset(s,0,n)
#define bcopy(src,dest,n) memmove(dest,src,n)
#define bcmp(a,b,n) memcmp(a,b,n)
#define index(a,b) strchr(a,b)
#define rindex(a,b) strrchr(a,b)
#endif

#endif
