#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H

#include <linux/types.h>
#include <linux/posix_types.h>
#include <linux/socket.h>

#ifdef __GNUC__
#define __quad_t __quad_t
typedef long long int __quad_t;
typedef unsigned long long int __u_quad_t;
#else
#warning no long long support - no 64 bit file I/O support
typedef long int __quad_t;
typedef unsigned long int __u_quad_t;
#endif


#ifdef __quad_t
typedef __quad_t off64_t;
typedef __u_quad_t ino64_t;
typedef __quad_t blkcnt64_t;
#endif

typedef long int fpos_t;

#define socklen_t socklen_t
typedef unsigned int socklen_t;

typedef long int blksize_t;
typedef long int blkcnt_t;

#endif
