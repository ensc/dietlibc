#include <asm/byteorder.h>

#undef htonl
#ifdef __i386__
unsigned long int htonl(unsigned long int netlong) {
  return ___arch__swab32(netlong);
}
#else
unsigned long int htonl(unsigned long int netlong) {
  return netlong;
}
#endif
