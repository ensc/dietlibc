#include <asm/byteorder.h>

#undef htons
#ifdef __i386__
unsigned short int htons(unsigned short int netshort) {
  return ___arch__swab16(netshort);
}
#else
unsigned short int htons(unsigned short int netshort) {
  return netshort;
}
#endif
