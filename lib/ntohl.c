#include <linux/byteorder/generic.h>
#include <asm/byteorder.h>

#undef ntohl
#ifdef __i386__
unsigned long int ntohl(unsigned long int netlong) {
  return ___arch__swab32(netlong);
}
#else
unsigned long int ntohl(unsigned long int netlong) {
  return netlong;
}
#endif
