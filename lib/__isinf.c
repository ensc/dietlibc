#include <math.h>

int isinf(double d) {
  unsigned long long *x=(unsigned long long *)&d;
  return (*x==0x7FF0000000000000ll?1:*x==0xFFF0000000000000?-1:0);
}
int __isinf(double d) __attribute__((alias("isinf")));

#if 0
TestFromIeeeExtended("7FFF0000000000000000");   /* +infinity */
TestFromIeeeExtended("FFFF0000000000000000");   /* -infinity */
TestFromIeeeExtended("7FFF8001000000000000");   /* Quiet NaN(1) */
TestFromIeeeExtended("7FFF0001000000000000");   /* Signalling NaN(1) */
TestFromIeeeExtended("3FFFFEDCBA9876543210");   /* accuracy test */
#endif
