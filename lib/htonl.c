#include <endian.h>

unsigned long int htonl(unsigned long int hostlong) {
#if __BYTE_ORDER==__LITTLE_ENDIAN
  return (hostlong>>24) | ((hostlong>>16)&0xff) |
	 ((hostlong>>8)&0xff) | (hostlong&0xff);
#else
  return hostlong;
#endif
}

unsigned long int ntohl(unsigned long int hostlong) __attribute__((weak,alias("htonl")));
