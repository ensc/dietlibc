#include <endian.h>

unsigned short int htons(unsigned short int hostshort) {
#if __BYTE_ORDER==__LITTLE_ENDIAN
  return ((hostshort>>8)&0xff) | (hostshort<<8);
#else
  return hostshort;
#endif
}

unsigned short int ntohs(unsigned short int hostshort) __attribute__((weak,alias("htons")));
