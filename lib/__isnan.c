#include <math.h>

int isnan(double d) {
  unsigned long long *x=(unsigned long long *)&d;
  return (*x==0x7FF8002000000000ll || *x==0x7FF0002000000000);
}
int __isnan(double d) __attribute__((alias("isnan")));

#if 0
TestFromIeeeExtended("7FFF0000000000000000");   /* +infinity */
TestFromIeeeExtended("FFFF0000000000000000");   /* -infinity */
TestFromIeeeExtended("7FFF8001000000000000");   /* Quiet NaN(1) */
TestFromIeeeExtended("7FFF0001000000000000");   /* Signalling NaN(1) */
TestFromIeeeExtended("3FFFFEDCBA9876543210");   /* accuracy test */
#endif
