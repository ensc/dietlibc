#include <endian.h>

#if __WORDSIZE != 64
long long int llabs(long long int i) { return i>=0?i:-i; }
#endif
