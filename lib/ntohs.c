#include <linux/byteorder/generic.h>
#include <asm/byteorder.h>

#undef ntohs
#ifdef __i386__
unsigned short int ntohs(unsigned short int netshort) {
  return ___arch__swab16(netshort);
}
#else
unsigned short int ntohs(unsigned short int netshort) {
  return netshort;
}
#endif
