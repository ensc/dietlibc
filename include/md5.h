#ifndef _MD5_H
#define _MD5_H

#include <sys/types.h>

/* 
 *  currently the code only works if sizeof(char)==8.
 *  This is also true for the original code.
 *  A lot of sophisticated changes must be done to support 32 bit DSPs
 */

/* defining the MD5 context structure */
typedef struct  MD5Context {
    uint32_t  state   [4];	/* state (ABCD) */
    uint32_t  count   [2];	/* number of bits, modulo 2^64 (least significant word first) */
    uint8_t   buffer [64];	/* input buffer for incomplete buffer data */
} MD5_CTX;

void   MD5Init        ( MD5_CTX* ctx );
void   MD5Update      ( MD5_CTX* ctx, const uint8_t* buf, size_t len );
void   MD5Final       ( uint8_t digest[16], MD5_CTX* ctx );	/* nice idea of the interface designer: here the CTX is the last argument */
void   MD5Transform   ( uint32_t state[4], const uint8_t block[64] );
void   __MD5Transform ( uint32_t state[4], const uint8_t block[64], size_t repeat );

char*  md5crypt       ( const char* pw, const char* salt );

#endif
