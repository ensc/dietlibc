#include <md5.h>
#include <endian.h>

#ifdef __i386__
/*
 *  Can be defined if:
 *    - md5asm.S is available
 *    - CPU is a i386
 */
# define USE_INTEL_ASM
#endif

/*
 *  Can be defined if:
 *    - CHAR_BIT = 8
 *    - sizeof(word) = 4
 *    - little endian machine
 */
#if (__WORDSIZE == 32) && (__BYTE_ORDER == __LITTLE_ENDIAN)
# define NO_ENCODE_DECODE
#endif

#define S11  7
#define S12 12
#define S13 17
#define S14 22
#define S21  5
#define S22  9
#define S23 14
#define S24 20
#define S31  4
#define S32 11
#define S33 16
#define S34 23
#define S41  6
#define S42 10
#define S43 15
#define S44 21

#define F(x, y, z)  (((x) & (y)) | (~(x) & (z)))
#define G(x, y, z)  (((x) & (z)) | ((y) & ~(z)))
#define H(x, y, z)  ((x) ^ (y) ^ (z))
#define I(x, y, z)  ((y) ^ ((x) | ~(z)))

#define ROTATE_LEFT(x, n)  (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) { (a) += F (b, c, d) + (x) + (uint32_t)(ac); (a) = ROTATE_LEFT (a, s); (a) += (b); }
#define GG(a, b, c, d, x, s, ac) { (a) += G (b, c, d) + (x) + (uint32_t)(ac); (a) = ROTATE_LEFT (a, s); (a) += (b); }
#define HH(a, b, c, d, x, s, ac) { (a) += H (b, c, d) + (x) + (uint32_t)(ac); (a) = ROTATE_LEFT (a, s); (a) += (b); }
#define II(a, b, c, d, x, s, ac) { (a) += I (b, c, d) + (x) + (uint32_t)(ac); (a) = ROTATE_LEFT (a, s); (a) += (b); }


#ifndef NO_ENCODE_DECODE
/* Encodes input (32 bit) into output (8 bit). len is in 32 bit units */

static void  Encode ( uint8_t* dst, const uint32_t* src, size_t length )
{
    uint32_t  val;
    
    while ( length-- ) {
        val = *src++;
	*dst++ = (uint8_t) ((val >>  0) & 0xff);
	*dst++ = (uint8_t) ((val >>  8) & 0xff);
	*dst++ = (uint8_t) ((val >> 16) & 0xff);
	*dst++ = (uint8_t) ((val >> 24) & 0xff);
    }
}


/* Decodes input (8 bit) into output (32 bit). len is in 32 bit units */

static void  Decode ( uint32_t* dst, const uint8_t* src, size_t length )
{
    uint32_t  val;
    
    while ( length-- ) {
	val  = (uint32_t)(*src++) <<  0;
	val |= (uint32_t)(*src++) <<  8;
	val |= (uint32_t)(*src++) << 16;
	val |= (uint32_t)(*src++) << 24;
	*dst++ = val;
    }
}

#endif

#ifdef USE_INTEL_ASM

extern 
void  __MD5Transform ( uint32_t state[4], const uint8_t block[64], size_t repeat );

#else

void  __MD5Transform ( uint32_t state[4], const uint8_t block[64], size_t repeat )
{
#ifdef NO_ENCODE_DECODE
    register const uint32_t*  x;
#else    
    uint32_t                  x [16];
#endif    
    register uint32_t         a;
    register uint32_t         b;
    register uint32_t         c;
    register uint32_t         d;
    
    if (repeat == 0)
        return;
    
rep:
    
    a = state [0];
    b = state [1];
    c = state [2];
    d = state [3];
    
#ifdef NO_ENCODE_DECODE
    x = (const uint32_t*)block;
#else    
    Decode ( x, block, 16 );
#endif    
    
    /* Round 1 */
    FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /*  1 */
    FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /*  2 */
    FF (c, d, a, b, x[ 2], S13, 0x242070db); /*  3 */
    FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /*  4 */
    FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /*  5 */
    FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /*  6 */
    FF (c, d, a, b, x[ 6], S13, 0xa8304613); /*  7 */
    FF (b, c, d, a, x[ 7], S14, 0xfd469501); /*  8 */
    FF (a, b, c, d, x[ 8], S11, 0x698098d8); /*  9 */
    FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
    FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
    FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
    FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
    FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
    FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
    FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */
    
    /* Round 2 */
    GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
    GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
    GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
    GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
    GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
    GG (d, a, b, c, x[10], S22, 0x02441453); /* 22 */
    GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
    GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
    GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
    GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
    GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
    GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
    GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
    GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
    GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
    GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */
    
    /* Round 3 */
    HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
    HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
    HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
    HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
    HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
    HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
    HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
    HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
    HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
    HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
    HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
    HH (b, c, d, a, x[ 6], S34, 0x04881d05); /* 44 */
    HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
    HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
    HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
    HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */
    
    /* Round 4 */
    II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
    II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
    II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
    II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
    II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
    II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
    II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
    II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
    II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
    II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
    II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
    II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
    II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
    II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
    II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
    II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */
    
    state [0] += a;
    state [1] += b;
    state [2] += c;
    state [3] += d;
    
    block += 64;
    
    if (--repeat ) goto rep;
    
    /* Zeroize sensitive information */
#ifndef NO_ENCODE_DECODE
    memset ( x, 0, sizeof (x) );
#endif    
}

void  MD5Init ( MD5_CTX* context )
{
    context->count[0] = 0;
    context->count[1] = 0;
    
    context->state[0] = 0x67452301;		/* Load magic initialization constants. */
    context->state[1] = 0xefcdab89;
    context->state[2] = 0x98badcfe;
    context->state[3] = 0x10325476;
}


/* MD5 block update operation: Continues an MD5 message-digest operation, processing another message block, and updating the context */

void  MD5Update ( MD5_CTX* context, const uint8_t* input, size_t inputBytes )
{
    uint32_t  i;
    uint32_t  ByteIndex;
    uint32_t  partLen;
    uint32_t  len;

    /* Compute number of bytes mod 64 */
    ByteIndex = (context->count[0] >> 3) & 0x3F;
    
    /* Update number of bits: count += 8 * inputBytes */
    if ( (context->count[0] += inputBytes << 3 ) < (inputBytes << 3) )
	context->count[1]++;
    context->count[1] += inputBytes >> (32-3);
    
    partLen = 64 - ByteIndex;
    
    /* Transform as many times as possible. */
    if ( inputBytes >= partLen ) {
	memcpy ( context->buffer + ByteIndex, input, partLen );
	__MD5Transform ( context->state, context->buffer, 1 );
        len = (inputBytes - partLen) / 64;
	__MD5Transform ( context->state, input+partLen, len );
	i = partLen + 64 * len;
	ByteIndex = 0;
    } else {
        i = 0;
    }
    
    /* Buffer remaining input */
    memcpy ( context->buffer + ByteIndex, input + i, inputBytes - i );
}

#endif

void  MD5Transform ( uint32_t state[4], const uint8_t block[64] ) 
{
    __MD5Transform ( state, block, 1 );
}

void  MD5Final ( uint8_t digest[16], MD5_CTX* context ) 
{
    static uint8_t  padding [64];
    uint8_t         bits     [8];
    uint32_t        index;
    uint32_t        padLen;
    
    /* Save number of bits */
#ifdef NO_ENCODE_DECODE
    memcpy ( bits, context->count,  8 );
#else    
    Encode ( bits, context->count,  2 );
#endif    
    
    /* Pad out to 56 mod 64. */
    index  = (context->count[0] >> 3) & 0x3f;
    padLen = 56 - index;
    if ( (signed int)padLen <= 0 )
        padLen += 64;
    padding [0] = 0x80;
    MD5Update ( context, padding, padLen );
    
    /* Append length (before padding) */
    MD5Update ( context, bits, 8 );
    
    /* Store state in digest */
#ifdef NO_ENCODE_DECODE
    memcpy ( digest, context->state, 16 );
#else
    Encode ( digest, context->state,  4 );
#endif    
    
    /* Zeroize sensitive information. */
    memset ( context, 0, sizeof (*context) );
}

/* end of md5.c */
