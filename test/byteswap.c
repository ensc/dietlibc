#include <byteswap.h>
#include <stdio.h>

main() {
  printf("%x %x\n",bswap_16(0x1234),bswap_16(0x5678));
  printf("%lx\n",bswap_32(0x12345678));
  printf("%qx\n",bswap_64(0x123456789ABCDEFull));
}
