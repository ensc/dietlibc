#include <endian.h>

unsigned long int htonl(unsigned long int hostlong) {
#if __BYTE_ORDER==__LITTLE_ENDIAN
  return (hostlong>>24) | ((hostlong&0xff0000)>>8) |
	  ((hostlong&0xff00)<<8) | (hostlong<<24);
#else
  return hostlong;
#endif
}

unsigned long int ntohl(unsigned long int hostlong) __attribute__((weak,alias("htonl")));
