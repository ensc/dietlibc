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

size_t strcspn(const char *s, const char *reject) __THROW;

void* memset(void *s, int c, size_t n) __THROW;

#undef NULL
#define NULL (void*)0

#define bzero(s,n) memset(s,0,n)
#define bcmp(s1,s2,n) memcmp(s1,s2,n)
#define bcopy(src,dest,n) memcpy(dest,src,n)

char *strerror(int errnum) __THROW;
char *strsignal(int signum) __THROW;

#endif
