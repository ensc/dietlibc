#ifndef _DIETFEATURES_H
#define _DIETFEATURES_H

/* feel free to comment some of these out to reduce code size */

#define WANT_FLOATING_POINT_IN_PRINTF
#define WANT_FLOATING_POINT_IN_SCANF
#define WANT_CHARACTER_CLASSES_IN_SCANF
#define WANT_NULL_PRINTF
#define WANT_LONGLONG_PRINTF
#define WANT_LONGLONG_SCANF

/* 128 or 2048 bytes buffer size? */
/* #define WANT_SMALL_STDIO_BUFS */

/* want fread to read() directly if size of data is larger than buffer?
 * This costs a few bytes but is worth it if the application is already
 * buffering. */
#define WANT_FREAD_OPTIMIZATION

/* this is only for meaningful for ttyname and sysconf_cpus so far */
#define SLASH_PROC_OK

/* use errno_location instead of errno */
#define WANT_THREAD_SAFE

/* make the startcode, etc. dynamic aware ({con,de}structors) */
/* #define WANT_DYNAMIC */

/* do you want smaller or faster string routines? */
/* #define WANT_FASTER_STRING_ROUTINES */

/* define this to have strncpy zero-fill and not just zero-terminate the
 * string */
/* #define WANT_FULL_POSIX_COMPAT */

/* read the comment in lib/strncat.c for an explanation */
/* #define WANT_NON_COMPLIANT_STRNCAT */

#define WANT_LINKER_WARNINGS

/* you need to define this if you want to run your programs with large
 * file support on kernel 2.2 or 2.0 */
#define WANT_LARGEFILE_BACKCOMPAT

/* do you want localtime(3) to read /etc/localtime?
 * Needed for daylight saving time etc. */
#define WANT_TZFILE_PARSER

/* do you want the DNS routines to parse and use "domain" and "search"
 * lines from /etc/resolv.conf?  Normally not used on boot floppies and
 * embedded environments. */
#define WANT_FULL_RESOLV_CONF

/* do you want gethostbyname and friends to consult /etc/hosts? */
#define WANT_ETC_HOSTS

/* do you want math functions high precision rather than fast/small? */
#define WANT_HIGH_PRECISION_MATH

/* do you want support for matherr? */
#define WANT_MATHERR

/* do you want crypt(3) to use MD5 if the salt starts with "$1$"? */
#define WANT_CRYPT_MD5

/* do you want diet to include a safeguard dependency to make linking
 * against glibc fail? */
#define WANT_SAFEGUARD

/* dy you want that malloc(0) return a pointer to a "zero-length" object
 * that is realloc-able; means realloc(..,size) gives a NEW object (like a
 * call to malloc(size)).
 * WARNING: this violates C99 */
/* #define WANT_MALLOC_ZERO */


/* stop uncommenting here ;-) */
#ifndef WANT_FASTER_STRING_ROUTINES
#define WANT_SMALL_STRING_ROUTINES
#endif

#ifdef WANT_THREAD_SAFE
#ifndef __ASSEMBLER__
#define errno (*__errno_location())
#define _REENTRANT
#endif
#endif

#ifdef __DYN_LIB
/* with shared libraries you MUST have a dynamic aware startcode */
#ifndef WANT_DYNAMIC
#define WANT_DYNAMIC
#endif
/* saveguard crashes with shared objects ... */
#ifdef WANT_SAFEGUARD
#undef WANT_SAFEGUARD
#endif
#endif

#endif
