#ifndef _MD5_H
#define _MD5_H

#include <sys/types.h>

/* MD5 context. */
typedef struct MD5Context {
    uint32_t state[4];		/* state (ABCD) */
    uint32_t count[2];		/* number of bits, modulo 2^64 (least significant word first) */
    unsigned char buffer[64];	/* input buffer */
} MD5_CTX;

void   MD5Init (MD5_CTX *ctx);
void   MD5Update (MD5_CTX *ctx, const unsigned char *buf, size_t len);
void   MD5Final (unsigned char digest[16], MD5_CTX *ctx);
void   MD5Transform (uint32_t state[4], const unsigned char block[64]);
void  __MD5Transform(uint32_t state[4], const unsigned char block[64], size_t repeat);

char* md5crypt(const char* pw, const char* salt);

#endif
